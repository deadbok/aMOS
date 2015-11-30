#ifndef LOADER_DISK_H
#define LOADER_DISK_H

#include "common.h"

/* These are the error codes returned in AH by the BIOS disk functions */

#define N_DISK_ERRORS	34

static struct error	disk_errors[] = {
	{0x00, "OK"},
	{0x01, "invalid function or parameter"},
	{0x02, "address mark not found"},
	{0x03, "disk write-protected"},
	{0x04, "sector not found/read error"},
	{0x05, "reset failed"},
	{0x05, "data did not verify correctly"},
	{0x06, "disk changed"},
	{0x07, "drive parameter activity failed"},
	{0x08, "DMA overrun"},
	{0x09, "data boundary error"},
	{0x0a, "bad sector detected"},
	{0x0b, "bad track detected"},
	{0x0c, "unsupported track or invalid media"},
	{0x0d, "invalid number of sectors on format"},
	{0x0e, "control data address mark detected"},
	{0x0f, "DMA arbitration level out of range"},
	{0x10, "uncorrectable CRC or ECC error on read"},
	{0x11, "data ECC corrected"},
	{0x20, "controller failure"},
	{0x31, "no media in drive"},
	{0x32, "incorrect drive type stored in CMOS"},
	{0x40, "seek failed"},
	{0x80, "timeout (not ready)"},
	{0xaa, "drive not ready"},
	{0xb0, "volume not locked in drive"},
	{0xb1, "volume locked in drive"},
	{0xb2, "volume not removable"},
	{0xb3, "volume in use"},
	{0xb4, "lock count exceeded"},
	{0xb5, "valid eject request failed"},
	{0xbb, "undefined error"},
	{0xcc, "write fault"},
	{0xe0, "status register error"},
	{0xff, "sense operation failed"}
};

#endif