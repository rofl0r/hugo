#ifndef __ASPI_H__
#define __ASPI_H__

#define ASPI_MKID(hostad,target,lun)	(((hostad)<<6)|(((target)&7)<<3)|((lun)&7))
#define ASPI_ID2LUN(id)			((id)&0x07)
#define ASPI_ID2TARGET(id)		(((id)>>3)&0x07)
#define ASPI_ID2HOSTAD(id)		(((id)>>6)&0x07)

#define ASPI_RW_NODATA	0
#define ASPI_RW_READ	1
#define ASPI_RW_WRITE	2
#define ASPI_RW_BOTH	3

extern long aspi_buffer_length;	/* defaults to 128K, set before aspi_init, multiple of 16 */
extern char aspi_sense[];

/* You must call this before anything else.  Returns number of Host Adapters, or -1 on error. */
extern int aspi_init(void);

/* You must call this before exiting.  Returns 0 on success */
extern int aspi_close(void);

/* Fast query the device type */
extern int aspi_device_type(int _aspi_id);

/* General Purpose SCSI command */
extern int aspi_exec(int _aspi_id, void *_buf, int _buflen, int _rw, char *_cdb, int _cdblen);

#endif /* __ASPI_H__ */
