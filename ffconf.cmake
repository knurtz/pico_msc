# Custom definitions otherwise found in FatFS ffconf.h
# The compiler will always prioritize ffconf.h from the FatFS submodule folder over a custom one in a different folder.
# By defining FFCONF_DEF as a compiler definition, FatFS will not include ffconf.h at all, so we have to provide all (necessary) defines ourselves.

target_compile_definitions(pico_msc INTERFACE
	FFCONF_DEF=5380

	FF_FS_READONLY=1        # for now - todo for later: enable write access by application when USB MSC is not in use.

	FF_FS_MINIMIZE=0
	FF_USE_FIND=0

	FF_USE_MKFS=0

	FF_USE_FASTSEEK=0

	FF_USE_EXPAND=0

	FF_USE_CHMOD=0

	FF_USE_LABEL=1          # changed from original

	FF_USE_FORWARD=0

	FF_USE_STRFUNC=0
	FF_PRINT_LLI=0
	FF_PRINT_FLOAT=0
	FF_STRF_ENCODE=3
	FF_CODE_PAGE=850        # changed from original

	FF_USE_LFN=1            # changed from original
	FF_MAX_LFN=255

	FF_LFN_UNICODE=1

	FF_LFN_BUF=255
	FF_SFN_BUF=12

	FF_FS_RPATH=0

	FF_VOLUMES=1

	FF_STR_VOLUME_ID=0

	FF_MULTI_PARTITION=0

	FF_MIN_SS=512
	FF_MAX_SS=512

	FF_LBA64=0

	FF_MIN_GPT=0x10000000

	FF_USE_TRIM=0

	FF_FS_TINY=0

	FF_FS_EXFAT=0

	FF_FS_NORTC=0
	FF_NORTC_MON=11
	FF_NORTC_MDAY=1
	FF_NORTC_YEAR=2024

	FF_FS_NOFSINFO=0

	FF_FS_LOCK=0

	FF_FS_REENTRANT=0
	FF_FS_TIMEOUT=1000
)