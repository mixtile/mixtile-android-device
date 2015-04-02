#ifndef CEDARX_STREAM_H_
#define CEDARX_STREAM_H_

//#define _FILE_OFFSET_BITS 64
//#define __USE_FILE_OFFSET64
//#define __USE_LARGEFILE64
//#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>

#include <CDX_Common.h>
#include <include_base/tsemaphore.h>
#include "stream.h"

#if (_FILE_OFFSET_BITS==64)
#define cdx_off_t long long
#else
#define cdx_off_t long long
#endif

/*   stream.h */
#define STREAM_BUFFER_SIZE 2048
#define VCD_SECTOR_SIZE 2352
#define VCD_SECTOR_OFFS 24
#define VCD_SECTOR_DATA 2324

typedef struct cdx_stream_info 
{
	const char *info;
	const char *name;
	const char *comment;

  CedarXDataSourceDesc  data_src_desc;

  //TODO:merge with data_src_desc above;
  //This variable is holy shit.
  CedarXDataSourceDesc  *another_data_src_desc;

  //below reserved are only used by cedarx internal
  void *reserved_0;
  void *reserved_1;

  //below reserved are used by customer
  void *reserved_usr_0;
  void *reserved_usr_1;

  FILE  *file_handle;

  CedarXExternFdDesc fd_desc;

  void* m3u8_context;

  CedarXStreamInfo *streaminfo; // streaminfo for cedarx-2.0

  int  (*seek)(struct cdx_stream_info *stream, cdx_off_t offset, int whence);
  cdx_off_t (*tell)(struct cdx_stream_info *stream);
  int  (*read)(void *ptr, size_t size, size_t nmemb, struct cdx_stream_info *stream);
  int  (*write)(const void *ptr, size_t size, size_t nmemb, struct cdx_stream_info *stream);
  int  (*write2)(void *bs_info, struct cdx_stream_info *stream);
  long long (*getsize)(struct cdx_stream_info *stream);
  int (*destory)(struct cdx_stream_info *stm_info);
  int (*decrypt)(void *ptr, size_t size, int pkt_type, struct cdx_stream_info *stream);
  //below two function used for m3u/ts
  long long (*seek_to_time)(struct cdx_stream_info *stream, long long us);
  long long (*get_total_duration)(struct cdx_stream_info *stream);

	void (*reset_stream)(struct cdx_stream_info *stream);
	int (*control_stream)(struct cdx_stream_info * stream, void *arg, int cmd);
	int (*extern_writer)(void *parent, void *bs_info);

	int  (*seek_segment)(struct cdx_stream_info *stream, cdx_off_t offset, int whence, int type);
	cdx_off_t (*tell_segment)(struct cdx_stream_info *stream, int type);
	int  (*read_segment)(void *ptr, size_t size, size_t nmemb, struct cdx_stream_info *stream, int type);
} cdx_stream_info_t;

extern struct cdx_stream_info *create_stream_handle(CedarXDataSourceDesc *datasource_desc);
extern void destory_stream_handle(struct cdx_stream_info *stm_info);
extern struct cdx_stream_info *create_outstream_handle(CedarXDataSourceDesc *datasource_desc);
void destroy_outstream_handle(struct cdx_stream_info *stm_info);


static inline int cdx_seek_segment(struct cdx_stream_info *stream, cdx_off_t offset, int whence, int type)
{
	//LOGD("cdx_seek offset = %lld, whence = %d", offset, whence);
	return stream->seek_segment(stream, offset, whence, type);
}

static inline cdx_off_t cdx_tell_segment(struct cdx_stream_info *stream, int type)
{
	return stream->tell_segment(stream, type);
}

static inline int cdx_read_segment(void *ptr, size_t size, size_t nmemb, struct cdx_stream_info *stream, int type)
{
	return stream->read_segment(ptr, size, nmemb,stream, type);
}



static inline int cdx_seek(struct cdx_stream_info *stream, cdx_off_t offset, int whence)
{
	return stream->seek(stream, offset, whence);
}

static inline cdx_off_t cdx_tell(struct cdx_stream_info *stream)
{
	return stream->tell(stream);
}

static inline int cdx_read(void *ptr, size_t size, size_t nmemb, struct cdx_stream_info *stream)
{
	return stream->read(ptr, size, nmemb,stream);
}

static inline int cdx_write(const void *ptr, size_t size, size_t nmemb, struct cdx_stream_info *stream)
{
	return stream->write(ptr, size, nmemb,stream);
}

static inline int cdx_write2(void *bs_info, struct cdx_stream_info *stream)
{
	return stream->write2(bs_info,stream);
}

#endif /* CEDAR_DEMUX_H_ */