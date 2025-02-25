/*
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */
/* Some sections rewritten feb 2025.  All macros added, most comments in code added. */

#include "pce.h"
#include "utils.h"

/* Control codes */
#define CMD_READ_SECTOR 0x08
#define CMD_PLAY_AUDIO 0xD8
#define CMD_STOP_AUDIO 0xD9
#define CMD_PAUSE_AUDIO 0xDA
#define CMD_GET_DIR_INFO 0xDE
#define STATUS_CMD_ARG_RECEIVED 0xD0
#define STATUS_CMD_COMPLETE 0xC8
#define STATUS_PLAY_AUDIO 0xD8
#define STATUS_RESET 0x40

/* Function helper macros */
#define SET_CD_PORT_1800(value) io.cd_port_1800 = value
#define HANDLE_ADPCM_PTR(ptr, value) io.adpcm_##ptr = value
#define HANDLE_CD_EMULATION_RESET() \
    switch (CD_emulation) { \
        case 1: \
            if (osd_cd_init(ISO_filename) != 0) { \
                Log("CD rom drive couldn't be initialised\n"); \
                exit(4); \
            } \
            break; \
        case 2: \
        case 3: \
        case 4: \
            fill_cd_info(); \
            break; \
        case 5: \
            fill_HCD_info(ISO_filename); \
            break; \
    } \
    Wr6502(0x222D, 1); \
    io.cd_port_1804 = V

/* File global variables */
UChar pce_cd_cmdcnt;
UInt32 pce_cd_sectoraddy;
UChar pce_cd_sectoraddress[3];
UChar pce_cd_temp_dirinfo[4];
UChar pce_cd_temp_play[4];
UChar pce_cd_temp_stop[4];
UChar pce_cd_dirinfo[4];
extern UChar pce_cd_adpcm_trans_done;
UChar cd_port_180b = 0;
UChar cd_fade;

static void pce_cd_set_sector_address(void)
{
	pce_cd_sectoraddy = pce_cd_sectoraddress[0] << 16;
	pce_cd_sectoraddy += pce_cd_sectoraddress[1] << 8;
	pce_cd_sectoraddy += pce_cd_sectoraddress[2];
}

static void pce_cd_handle_command(void)
{
	if (pce_cd_cmdcnt)
	{
		if (--pce_cd_cmdcnt)
			io.cd_port_1800 = STATUS_CMD_ARG_RECEIVED;
		else
			io.cd_port_1800 = STATUS_CMD_COMPLETE;

		switch (pce_cd_curcmd)
		{
		case CMD_READ_SECTOR:
			// Handle the command to read a sector from the CD
			if (!pce_cd_cmdcnt)
			{
				cd_sectorcnt = io.cd_port_1801;

				if (cd_sectorcnt == 0)
				{
					fprintf(stderr, "Error: cd_sectorcnt is 0. No sectors to read. Check CD initialization or command sequence.");
					Log("Error: cd_sectorcnt is 0. No sectors to read. Check CD initialization or command sequence.");
				}

				pce_cd_set_sector_address();
				pce_cd_read_sector();
			}
			else
			{
				pce_cd_sectoraddress[3 - pce_cd_cmdcnt] = io.cd_port_1801;
			}
			break;

		case CMD_PLAY_AUDIO:
			// Handle the command to play audio from the CD
			pce_cd_temp_play[pce_cd_cmdcnt] = io.cd_port_1801;

			if (!pce_cd_cmdcnt)
			{
				io.cd_port_1800 = STATUS_PLAY_AUDIO;
			}
			break;

		case CMD_STOP_AUDIO:
			// Handle the command to stop audio playback from the CD
			pce_cd_temp_stop[pce_cd_cmdcnt] = io.cd_port_1801;

			if (!pce_cd_cmdcnt)
			{
				io.cd_port_1800 = STATUS_PLAY_AUDIO;

				if ((pce_cd_temp_play[0] | pce_cd_temp_play[1] | pce_cd_temp_stop[0] | pce_cd_temp_stop[1]) == 0)
				{
					if (CD_emulation == 5)
						HCD_play_track(bcdbin[pce_cd_temp_play[2]], 1);
					else
						osd_cd_play_audio_track(bcdbin[pce_cd_temp_play[2]]);
				}
				else
				{
					if (CD_emulation == 5)
						HCD_play_sectors(Time2Frame(bcdbin[pce_cd_temp_play[2]], bcdbin[pce_cd_temp_play[1]], bcdbin[pce_cd_temp_play[0]]),
							Time2Frame(bcdbin[pce_cd_temp_stop[2]], bcdbin[pce_cd_temp_stop[1]], bcdbin[pce_cd_temp_stop[0]]),
							pce_cd_temp_stop[3] == 1);
					else
						osd_cd_play_audio_range(bcdbin[pce_cd_temp_play[2]], bcdbin[pce_cd_temp_play[1]], bcdbin[pce_cd_temp_play[0]],
							bcdbin[pce_cd_temp_stop[2]], bcdbin[pce_cd_temp_stop[1]], bcdbin[pce_cd_temp_stop[0]]);
				}
			}
			break;

		case CMD_GET_DIR_INFO:
			// Handle the command to get directory information from the CD
			if (pce_cd_cmdcnt)
			{
				pce_cd_temp_dirinfo[pce_cd_cmdcnt] = io.cd_port_1801;
			}
			else
			{
				pce_cd_temp_dirinfo[0] = io.cd_port_1801;

#ifdef CD_DEBUG
				pce_cd_temp_dirinfo[0], pce_cd_temp_dirinfo[1], pce_cd_temp_dirinfo[2], pce_cd_temp_dirinfo[3]);
#endif

				switch (pce_cd_temp_dirinfo[1])
				{
				case 0:
					// Get the number of tracks on the CD
					switch (CD_emulation)
					{
					case 2:
					case 3:
					case 4:
						pce_cd_dirinfo[0] = binbcd[01];
						pce_cd_dirinfo[1] = binbcd[nb_max_track];
						break;
					case 1:
					{
						int first_track, last_track;
						osd_cd_nb_tracks(&first_track, &last_track);
						pce_cd_dirinfo[0] = binbcd[first_track];
						pce_cd_dirinfo[1] = binbcd[last_track];
					}
					break;
					case 5:
						pce_cd_dirinfo[0] = binbcd[HCD_first_track];
						pce_cd_dirinfo[1] = binbcd[HCD_last_track];
						break;
					}

					cd_read_buffer = pce_cd_dirinfo;
					pce_cd_read_datacnt = 2;
					break;

				case 2:
					// Get the start time and type of a specific track
					switch (CD_emulation)
					{
					case 2:
					case 3:
					case 4:
					case 5:
						pce_cd_dirinfo[0] = CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].beg_min;
						pce_cd_dirinfo[1] = CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].beg_sec;
						pce_cd_dirinfo[2] = CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].beg_fra;
						pce_cd_dirinfo[3] = CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].type;
						break;
					case 1:
					{
						int Min, Sec, Fra, Ctrl;
						osd_cd_track_info(bcdbin[pce_cd_temp_dirinfo[0]], &Min, &Sec, &Fra, &Ctrl);

						pce_cd_dirinfo[0] = binbcd[Min];
						pce_cd_dirinfo[1] = binbcd[Sec];
						pce_cd_dirinfo[2] = binbcd[Fra];
						pce_cd_dirinfo[3] = Ctrl;

						Log("The control byte of the audio track #%d is 0x%02X. This byte indicates the track type and control information. Verify if the track type is correct.\n",
							bcdbin[pce_cd_temp_dirinfo[0]], pce_cd_dirinfo[3]);
					}
					break;
					}

					pce_cd_read_datacnt = 3;
					cd_read_buffer = pce_cd_dirinfo;
					break;

				case 1:
					// Get the total length of the CD
					switch (CD_emulation)
					{
					case 1:
					{
						int min, sec, fra;
						osd_cd_length(&min, &sec, &fra);
						pce_cd_dirinfo[0] = binbcd[min];
						pce_cd_dirinfo[1] = binbcd[sec];
						pce_cd_dirinfo[2] = binbcd[fra];
					}
					break;
					default:
						pce_cd_dirinfo[0] = 0x25;
						pce_cd_dirinfo[1] = 0x06;
						pce_cd_dirinfo[2] = 0x00;
					}

					pce_cd_read_datacnt = 3;
					cd_read_buffer = pce_cd_dirinfo;
					break;
				}
			}
			break;
		}
	}
	else
	{
		switch (io.cd_port_1801)
		{
		case 0x00:
			// Handle the case where no command is issued
			io.cd_port_1800 = STATUS_PLAY_AUDIO;
			break;
		case CMD_READ_SECTOR:
			// Prepare to handle the command to read a sector from the CD
			pce_cd_curcmd = io.cd_port_1801;
			pce_cd_cmdcnt = 4;
			break;
		case CMD_PLAY_AUDIO:
			// Prepare to handle the command to play audio from the CD
			pce_cd_curcmd = io.cd_port_1801;
			pce_cd_cmdcnt = 4;
			break;
		case CMD_STOP_AUDIO:
			// Prepare to handle the command to stop audio playback from the CD
			pce_cd_curcmd = io.cd_port_1801;
			pce_cd_cmdcnt = 4;
			break;
		case CMD_PAUSE_AUDIO:
			// Handle the command to pause audio playback from the CD
			pce_cd_curcmd = io.cd_port_1801;
			pce_cd_cmdcnt = 0;

			if (CD_emulation == 1)
				osd_cd_stop_audio();
			else if (CD_emulation == 5)
				HCD_pause_playing();
			break;
		case CMD_GET_DIR_INFO:
			// Prepare to handle the command to get directory information from the CD
			io.cd_port_1800 = STATUS_CMD_ARG_RECEIVED;
			pce_cd_cmdcnt = 2;
			pce_cd_read_datacnt = 3;
			pce_cd_curcmd = io.cd_port_1801;
			break;
		}
	}
}

UChar pce_cd_handle_read_1800(UInt16 A)
{
	switch (A & 15)
	{
	case 0:
		return io.cd_port_1800;
	case 1:
	{
		UChar retval;

		if (cd_read_buffer)
		{
			retval = *cd_read_buffer++;
			if (pce_cd_read_datacnt == 2048)
			{
				pce_cd_read_datacnt--;
			}
			if (!pce_cd_read_datacnt)
				cd_read_buffer = 0;
		}
		else
		{
			retval = 0;
		}
		return retval;
	}

	case 2:
		return io.cd_port_1802;

	case 3:
	{
		static UChar tmp_res = 0x02;

		tmp_res = 0x02 - tmp_res;

		io.backup = DISABLE;

		return tmp_res | 0x20;
	}

	case 4:
		return io.cd_port_1804;

	case 5:
		return 0x50;

	case 6:
		return 0x05;

	case 0x0A:
		if (!io.adpcm_firstread)
			return PCM[io.adpcm_rptr++];
		else
		{
			io.adpcm_firstread--;
			return NODATA;
		}

	case 0x0B:
		return 0x00;
	case 0x0C:
		return 0x01;
	case 0x0D:
		return 0x00;

	case 8:
		if (pce_cd_read_datacnt)
		{
			UChar retval;

			if (cd_read_buffer)
				retval = *cd_read_buffer++;
			else
				retval = 0;

			if (!--pce_cd_read_datacnt)
			{
				cd_read_buffer = 0;
				if (!--cd_sectorcnt)
				{
					io.cd_port_1800 |= STATUS_CMD_COMPLETE;
					pce_cd_curcmd = 0;
				}
				else
				{
					pce_cd_read_sector();
				}
			}
			return retval;
		}
		break;
	}
	return 0;
}

void pce_cd_handle_write_1800(UInt16 A, UChar V)
{
	switch (A & 15)
	{
	case 7:
		io.backup = ENABLE;
		return;

	case 0:
		if (V == 0x81)
			SET_CD_PORT_1800(STATUS_CMD_ARG_RECEIVED);
		return;
	case 1:
		io.cd_port_1801 = V;
		if (!pce_cd_cmdcnt)
			switch (V)
			{
			case 0:
				return;
			case 3:
				return;
			case CMD_READ_SECTOR:
				return;
			case 0x81:
				SET_CD_PORT_1800(STATUS_RESET);
				return;
			case CMD_PLAY_AUDIO:
			case CMD_STOP_AUDIO:
				return;
			case CMD_PAUSE_AUDIO:
				return;
			case 0xDD:
				return;
			case CMD_GET_DIR_INFO:
				return;
			default:
				return;
			}
		return;

	case 2:
		if ((!(io.cd_port_1802 & 0x80)) && (V & 0x80))
		{
			io.cd_port_1800 &= ~STATUS_RESET;
		}
		else if ((io.cd_port_1802 & 0x80) && (!(V & 0x80)))
		{
			io.cd_port_1800 |= STATUS_RESET;

			if (pce_cd_adpcm_trans_done)
			{
				io.cd_port_1800 |= 0x10;
				pce_cd_curcmd = 0x00;
				pce_cd_adpcm_trans_done = 0;
			}

			if (io.cd_port_1800 & 0x08)
			{
				if (io.cd_port_1800 & 0x20)
				{
					io.cd_port_1800 &= ~0x80;
				}
				else if (!pce_cd_read_datacnt)
				{
					if (pce_cd_curcmd == CMD_READ_SECTOR)
					{
						if (!--cd_sectorcnt)
						{
							io.cd_port_1800 |= 0x10;
							pce_cd_curcmd = 0x00;
						}
						else
						{
							pce_cd_read_sector();
						}
					}
					else
					{
						if (io.cd_port_1800 & 0x10)
						{
							io.cd_port_1800 |= 0x20;
						}
						else
						{
							io.cd_port_1800 |= 0x10;
						}
					}
				}
				else
				{
					pce_cd_read_datacnt--;
				}
			}
			else
			{
				pce_cd_handle_command();
			}
		}

		io.cd_port_1802 = V;
		return;
	case 4:
		if (V & 2)
		{
			HANDLE_CD_EMULATION_RESET();
		}
		else
		{
			io.cd_port_1804 = V;
		}
		return;

	case 8:
		HANDLE_ADPCM_PTR(ptr.B.l, V);
		return;
	case 9:
		HANDLE_ADPCM_PTR(ptr.B.h, V);
		return;

	case 0x0A:
		PCM[io.adpcm_wptr++] = V;
		return;
	case 0x0B:
		if ((V & 2) && (!(cd_port_180b & 2)))
		{
			issue_ADPCM_dma();
			cd_port_180b = V;
			return;
		}

		if (!V)
		{
			io.cd_port_1800 &= ~0xF8;
			io.cd_port_1800 |= STATUS_PLAY_AUDIO;
		}

		cd_port_180b = V;
		return;

	case 0x0D:
		if ((V & 0x03) == 0x03)
		{
			io.adpcm_dmaptr = io.adpcm_ptr.W;
		}

		if (V & 0x04)
		{
			io.adpcm_wptr = io.adpcm_ptr.W;
		}

		if (V & 0x08)
		{
			io.adpcm_rptr = io.adpcm_ptr.W;
			io.adpcm_firstread = 2;
		}

		if (V & 0x80)
		{
		}
		else
		{
		}

		return;

	case 0xe:
		io.adpcm_rate = 32 / (16 - (V & 15));
		return;

	case 0xf:
		cd_fade = V;
		return;
	}
}
