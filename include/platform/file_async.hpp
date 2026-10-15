#pragma once
#include "file_acquisition.hpp"
namespace ttyd {
struct FileAsyncServices {
 FileAcquisitionServices file;
 // DVDMgrReadAsync child: scalar ignored, false retains this child.
 bool (*submit)(void*,uint32_t disc,uint32_t destination,uint32_t length,uint32_t offset,uint32_t callback)=nullptr;
 bool (*callback)(void*,uint32_t function,uint32_t entry)=nullptr;
};
struct FileAsyncCursor {
 FileAsyncServices owner;uint32_t phase=0,path=0,type=0,completion=0;
 uint32_t entry=0,next=0,attempt=0,trials=0,payload=0,disc=0,rounded=0,header=0,destination=0;
};
// Original fileAsync80189d44. Engaged scalar0 is a completed invocation, not
// a retained native child. Hit never acquires a reference. Services/bytes stay
// admitted and stationary through native holds; no cancellation is implied.
bool requestPackedFile(uint32_t path,uint32_t type,uint32_t completion,
 const FileAsyncServices&,FileAsyncCursor&,uint32_t& result);
// Literal DVDMgrReadAsync stores only; the worker/IO completion is separate.
void submitPackedDvdRead(uint32_t disc,uint32_t destination,uint32_t length,
 uint32_t offset,uint32_t callback,const EventMemoryServices&);
struct FileAsyncCompletionCursor {
 FileAsyncServices owner;uint32_t phase=0,result=0,info=0,entry=0,disc=0,payload=0,type=0,callback=0;
};
// Original dvdReadDoneCallBack8018a1a0, including unchecked no-match domain.
// Native caller admits a real pending owner before entering this source body.
bool completePackedFile(uint32_t result,uint32_t info,const FileAsyncServices&,FileAsyncCompletionCursor&);
}
