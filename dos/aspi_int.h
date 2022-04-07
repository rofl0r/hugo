#define PK __attribute__((packed))

/*
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
*/

#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned long

typedef void (*FARPROC)();
#define far

typedef BYTE *LPSRB;

#define SENSE_LEN		14		/*  Default sense buffer length */

#define SRB_DIR_SCSI		0x00		/*  Direction determined by SCSI command */
#define SRB_DIR_IN		0x08		/*  Transfer from SCSI target to host */
#define SRB_DIR_OUT		0x10		/*  Transfer from host to SCSI targetw */
#define SRB_POSTING		0x01		/*  Enable ASPI posting */

#define WM_ASPIPOST		0x4D42		/*  ASPI Post message */

/***************************************************************************
						 %%% ASPI Command Definitions %%%
***************************************************************************/
#define SC_HA_INQUIRY		0x00		/*  Host adapter inquiry */
#define SC_GET_DEV_TYPE		0x01		/*  Get device type */
#define SC_EXEC_SCSI_CMD	0x02		/*  Execute SCSI command */
#define SC_ABORT_SRB		0x03		/*  Abort an SRB */
#define SC_RESET_DEV		0x04		/*  SCSI bus device reset */

/***************************************************************************
								  %%% SRB Status %%%
***************************************************************************/
#define SS_PENDING		0x00		/*  SRB being processed */
#define SS_COMP			0x01		/*  SRB completed without error */
#define SS_ABORTED		0x02		/*  SRB aborted */
#define SS_ABORT_FAIL		0x03		/*  Unable to abort SRB */
#define SS_ERR 			0x04		/*  SRB completed with error */

#define SS_INVALID_CMD		0x80		/*  Invalid ASPI command */
#define SS_INVALID_HA		0x81		/*  Invalid host adapter number */
#define SS_NO_DEVICE		0x82		/*  SCSI device not installed */

#define SS_INVALID_SRB		0xE0		/*  Invalid parameter set in SRB */
#define SS_OLD_MANAGER		0xE1		/*  ASPI manager doesn't support Windows */
#define SS_ILLEGAL_MODE		0xE2		/*  Unsupported Windows mode */
#define SS_NO_ASPI		0xE3		/*  No ASPI managers resident */
#define SS_FAILED_INIT		0xE4		/*  ASPI for windows failed init */
#define SS_ASPI_IS_BUSY		0xE5		/*  No resources available to execute cmd */
#define SS_BUFFER_TO_BIG	0xE6		/*  Buffer size to big to handle! */

/***************************************************************************
							%%% Host Adapter Status %%%
***************************************************************************/
#define HASTAT_OK		0x00		/*  Host adapter did not detect an error */
#define HASTAT_SEL_TO		0x11		/*  Selection Timeout */
#define HASTAT_DO_DU		0x12		/*  Data overrun data underrun */
#define HASTAT_BUS_FREE		0x13		/*  Unexpected bus free */
#define HASTAT_PHASE_ERR	0x14		/*  Target bus phase sequence failure */

/***************************************************************************
			 %%% SRB - HOST ADAPTER INQUIRY - SC_HA_INQUIRY %%%
***************************************************************************/
typedef struct {

	BYTE	SRB_Cmd;			/*  ASPI command code = SC_HA_INQUIRY */
	BYTE	SRB_Status;			/*  ASPI command status byte */
	BYTE	SRB_HaId;			/*  ASPI host adapter number */
	BYTE	SRB_Flags;			/*  ASPI request flags */
	DWORD	SRB_Hdr_Rsvd;			/*  Reserved, MUST = 0 */
	BYTE	HA_Count;			/*  Number of host adapters present */
	BYTE	HA_SCSI_ID;			/*  SCSI ID of host adapter */
	BYTE	HA_ManagerId[16] PK;		/*  String describing the manager */
	BYTE	HA_Identifier[16] PK;		/*  String describing the host adapter */
	BYTE	HA_Unique[16] PK;		/*  Host Adapter Unique parameters */

} SRB_HAInquiry;

/* *************************************************************************** */
/* 			  %%% SRB - GET DEVICE TYPE - SC_GET_DEV_TYPE %%% */
/* *************************************************************************** */
typedef struct {

	BYTE	SRB_Cmd;			/*  ASPI command code = SC_GET_DEV_TYPE */
	BYTE	SRB_Status;			/*  ASPI command status byte */
	BYTE	SRB_HaId;			/*  ASPI host adapter number */
	BYTE	SRB_Flags;			/*  ASPI request flags */
	DWORD	SRB_Hdr_Rsvd PK;		/*  Reserved, MUST = 0 */
	BYTE	SRB_Target;			/*  Target's SCSI ID */
	BYTE	SRB_Lun;			/*  Target's LUN number */
	BYTE	SRB_DeviceType;			/*  Target's peripheral device type */

} SRB_GDEVBlock;

/* *************************************************************************** */
/* 		  %%% SRB - EXECUTE SCSI COMMAND - SC_EXEC_SCSI_CMD %%% */
/* *************************************************************************** */

typedef struct {				/*  Common prefix for CDBs */

	BYTE	SRB_Cmd;			/*  ASPI command code = SC_EXEC_SCSI_CMD */
	BYTE	SRB_Status;			/*  ASPI command status byte */
	BYTE	SRB_HaId;			/*  ASPI host adapter number */
	BYTE	SRB_Flags;			/*  ASPI request flags */
	DWORD	SRB_Hdr_Rsvd PK;		/*  Reserved, MUST = 0 */
	BYTE	SRB_Target;			/*  Target's SCSI ID */
	BYTE	SRB_Lun;			/*  Target's LUN number */
	DWORD	SRB_BufLen PK;			/*  Data Allocation Length */
	BYTE	SRB_SenseLen;			/*  Sense Allocation Length */
	WORD	SRB_BufPointerOfs PK;		/*  Data Buffer Pointer */
	WORD	SRB_BufPointerSeg PK;		/*  Data Buffer Pointer */
	DWORD	SRB_Rsvd1 PK;			/*  Reserved, MUST = 0 */
	BYTE	SRB_CDBLen;			/*  CDB Length = 6 */
	BYTE	SRB_HaStat;			/* 	Host Adapter Status */
	BYTE	SRB_TargStat;			/* 	Target Status */
	FARPROC SRB_PostProc PK;		/*  Post routine */

	BYTE	SRB_Rsvd2[34] PK;		/*  Reserved, MUST = 0 */

	BYTE	cptr[12+SENSE_LEN] PK;
} SRB_ExecSCSICmd;

/* *************************************************************************** */
/* 				  %%% SRB - ABORT AN SRB - SC_ABORT_SRB %%% */
/* *************************************************************************** */
typedef struct {

	BYTE	SRB_Cmd;			/*  ASPI command code = SC_ABORT_SRB */
	BYTE	SRB_Status;			/*  ASPI command status byte */
	BYTE	SRB_HaId;			/*  ASPI host adapter number */
	BYTE	SRB_Flags;			/*  ASPI request flags */
	DWORD	SRB_Hdr_Rsvd PK;		/*  Reserved, MUST = 0 */
	LPSRB	SRB_ToAbort PK;			/*  Pointer to SRB to abort */

} SRB_Abort;

/* *************************************************************************** */
/* 				%%% SRB - BUS DEVICE RESET - SC_RESET_DEV %%% */
/* *************************************************************************** */
typedef struct {

	BYTE	SRB_Cmd;			/*  ASPI command code = SC_RESET_DEV */
	BYTE	SRB_Status;			/*  ASPI command status byte */
	BYTE	SRB_HaId;			/*  ASPI host adapter number */
	BYTE	SRB_Flags;			/*  ASPI request flags */
	DWORD	SRB_Hdr_Rsvd PK;		/*  Reserved, MUST = 0 */
	BYTE	SRB_Target;			/*  Target's SCSI ID */
	BYTE	SRB_Lun;			/*  Target's LUN number */
	BYTE	SRB_ResetRsvd1[14] PK;		/*  Reserved, MUST = 0 */
	BYTE	SRB_HaStat;			/*  Host Adapter Status */
	BYTE	SRB_TargStat;			/*  Target Status */
	FARPROC SRB_PostProc PK;		/*  Post routine */
	BYTE	SRB_ResetRsvd2[34] PK;		/*  Reserved, MUST = 0 */

} SRB_BusDeviceReset;
