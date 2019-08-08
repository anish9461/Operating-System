// filehdr.cc 
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the 
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the 
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect 
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector, 
//
//      Unlike in a real system, we do not keep track of file permissions, 
//	ownership, last modification date, etc., in the file header. 
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "filehdr.h"
#include "debug.h"
#include "synchdisk.h"
#include "main.h"
#include "blockfile.h"

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------

FileHeader::FileHeader() {
    for(int i = 0; i < NumDirect; ++i)
        dataSectors[i] = EMPTY_BLOCK;
    numBytes = 0;
    numSectors = 0;
}

bool
FileHeader::Allocate(PersistentBitmap *freeMap, int fileSize)
{ 
    numBytes = fileSize;
    int requirednumSectors  = divRoundUp(fileSize, SectorSize);
    if (freeMap->NumClear() < requirednumSectors)
	return FALSE;		// not enough space

    DoublyIndirectBlock *dblock;
    int allocated = 0;
    for (int i = 0; i < NumDirect && allocated < requirednumSectors; ++i) {
        dblock = new DoublyIndirectBlock();
        if(dataSectors[i] == EMPTY_BLOCK)
	        dataSectors[i] = freeMap->FindAndSet();
        else
        {
            dblock->FetchFrom(dataSectors[i]);
        }       
	// since we checked that there was enough free space,
	// we expect this to succeed
	ASSERT(dataSectors[i] != EMPTY_BLOCK);
    int output = dblock->Allocate(freeMap, requirednumSectors - allocated);
    ASSERT(output != -1);
    dblock->WriteBack(dataSectors[i]);                          // write doubly indirect block back
    allocated += output;                                     // decrease remaining sectors to be allocated
    delete dblock;
    }
    ASSERT(requirednumSectors - allocated <= 0);
    numBytes += fileSize;
    numSectors  += divRoundUp(fileSize, SectorSize);
    DEBUG('e', "file header allocated\n");
    return TRUE;
}

//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

void 
FileHeader::Deallocate(PersistentBitmap *freeMap)
{
    DoublyIndirectBlock *dblock;
    for (int i = 0,sector; i < NumDirect; ++i) {
	//ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
	    sector = dataSectors[i];
        if(sector == EMPTY_BLOCK)
            continue;
        
        ASSERT(freeMap->Test(sector));
        dblock = new DoublyIndirectBlock();
        dblock->FetchFrom(sector);
        dblock->Deallocate(freeMap);
        ASSERT(freeMap->Test(sector));
    freeMap->Clear(sector);
    delete dblock;
    }
}

//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk. 
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector)
{
    kernel->synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk. 
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector)
{
    kernel->synchDisk->WriteSector(sector, (char *)this); 
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

int
FileHeader::ByteToSector(int offset)
{
    //return(dataSectors[offset / SectorSize]);
    int vBlock = offset / SectorSize;
    DoublyIndirectBlock *dblock = new DoublyIndirectBlock();
    dblock->FetchFrom(dataSectors[vBlock / (MAX_BLOCKS * MAX_BLOCKS)]);
    int pBlock = dblock->ByteToSector(offset);
    ASSERT(pBlock >= 0 && pBlock < NumSectors);
    delete dblock;
    return pBlock;
}

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength()
{
    return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void
FileHeader::Print()
{
    int i, j, k;
    char *data = new char[SectorSize];

    printf("FileHeader contents.  File size: %d.  File blocks:\n", numBytes);
    for (i = 0; i < numSectors; i++)
	printf("%d ", dataSectors[i]);
    printf("\nFile contents:\n");
    for (i = k = 0; i < numSectors; i++) {
	kernel->synchDisk->ReadSector(dataSectors[i], data);
        for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
	    if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
		printf("%c", data[j]);
            else
		printf("\\%x", (unsigned char)data[j]);
	}
        printf("\n"); 
    }
    delete [] data;
}
