

#include "copyright.h"
#ifndef FILE_BLOCK_H
#define FILE_BLOCK_H

#include "disk.h"
#include "pbitmap.h"

#define MAX_BLOCKS (int) (SectorSize  / sizeof(int))
#define EMPTY_BLOCK -1


class IndirectBlock {
public:
	IndirectBlock();

	int Allocate(PersistentBitmap *bitMap, int numSectors);// Initialize a indirect block

    void Deallocate(PersistentBitmap *bitMap);  		// De-allocate this file's 
						//  data blocks

    void FetchFrom(int sectorNumber); 	// Initialize file header from disk
    void WriteBack(int sectorNumber); 	// Write modifications to file header
					//  back to disk

    int ByteToSector(int offset);	// Convert a byte offset into the file
					// to the disk sector containing
					// the byte

    // int FileLength();			// Return the length of the file 
					// in bytes

    void Print();			// Print the contents of the file.

private:
	int dataSectors[MAX_BLOCKS];
};



class DoublyIndirectBlock {
public:
	DoublyIndirectBlock();

	int Allocate(PersistentBitmap *bitMap, int numSectors);// Initialize a indirect block

    void Deallocate(PersistentBitmap *bitMap);  		// De-allocate this file's 
						//  data blocks

    void FetchFrom(int sectorNumber); 	// Initialize file header from disk
    void WriteBack(int sectorNumber); 	// Write modifications to file header
					//  back to disk

    int ByteToSector(int offset);	// Convert a byte offset into the file
					// to the disk sector containing
					// the byte

    // int FileLength();			// Return the length of the file 
					// in bytes

    void Print();			// Print the contents of the file.

private:
	int dataSectors[MAX_BLOCKS];
};




#endif	// FILE_BLOCK_H