// ------------------------------------------------------------------------
//
// Since finally woking on something not classified...
// I can make this public! Perhaps other might use or improve this.
// Note: Most of my utilities I prefer to remain private so i'll just copy-paste
//       individual items from my library.
//
// So the top of this file are items copied from my library.
// Run with -h to see help
// Rubiks Cube 3x3x3 One Look Last Layer (1LLL) Algorithm Generator
// Multi-Thread capable, iterates through all combinations without -r, run for 
// a little while to generate some algs, then copy all files in 1/* to
// an input file and then run with -f input.txt and -r to use the random turn mode.
// Still a work in progress... Currently we are looking at: 
//    (takes 4-5 minutes on an i3 to generate all 16416 1LLLs with 1 thread)
//    Then I run it in 12 threads across 2 machines 24/7, below is its progress 
//    slowly shortening the algorithms.
//
//  Move   Algorithm
//  Count  Counts
//  -----------------------------------------------------------------------------------------------------
//  1       0       0       0       0       0       0       0       0       0       0       0       0   
//  2       0       0       0       0       0       0       0       0       0       0       0       0   
//  3       0       0       0       0       0       0       0       0       0       0       0       0   
//  4       0       0       0       0       0       0       0       0       0       0       0       0   
//  5       0       0       0       0       0       0       0       0       0       0       0       0   
//  6       4       4       4       4       4       4       4       4       4       4       4       4   
//  7       22      22      22      22      22      22      22      22      22      22      22      22  
//  8       50      50      50      50      50      50      50      50      50      50      50      50  
//  9       94      94      94      94      94      94      94      94      94      94      94      94  
//  10      257     257     257     257     257     257     257     257     257     257     257     257 
//  11      891     891     891     891     891     891     891     891     891     891     891     891 
//  12      2360    2367    2374    2380    2384    2389    2393    2397    2404    2406    2414    2417
//  13      4281    4296    4313    4318    4331    4344    4350    4357    4391    4417    4453    4481
//  14      4907    4908    4910    4916    4914    4921    4920    4923    4954    4977    4977    4979
//  15      2867    2855    2833    2830    2815    2795    2792    2788    2746    2715    2687    2661
//  16      580     569     565     551     551     546     540     530     503     483     469     462 
//  17      98      98      98      98      98      98      98      98      95      95      93      93  
//  18      5       5       5       5       5       5       5       5       5       5       5       5   
//  19      0       0       0       0       0       0       0       0       0       0       0       0   
//  20      0       0       0       0       0       0       0       0       0       0       0       0   
//  21      0       0       0       0       0       0       0       0       0       0       0       0   
//  22      0       0       0       0       0       0       0       0       0       0       0       0   
//  23      0       0       0       0       0       0       0       0       0       0       0       0   
//  24      0       0       0       0       0       0       0       0       0       0       0       0   
//  25      0       0       0       0       0       0       0       0       0       0       0       0   
//  26      0       0       0       0       0       0       0       0       0       0       0       0   
//  27      0       0       0       0       0       0       0       0       0       0       0       0   
//  28      0       0       0       0       0       0       0       0       0       0       0       0   
//  29      0       0       0       0       0       0       0       0       0       0       0       0   
//  30      0       0       0       0       0       0       0       0       0       0       0       0   
//  
//  date    5/28    5/29    5/30    5/31    6/1     6/2     6/3     6/4     6/5     6/6     6/7     6/8
//
//-----------------------------------------------------------------------------------------------------

#define __UTIL_CPP__

int _CRT_glob = 0; // 0 turns off glob (wildcards are passed in as parameters rather than a file listing)

#undef __STRICT_ANSI__
#define WINDOWS
#define _CRT_SECURE_NO_WARNINGS //still does not supress warnings   
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <conio.h>
#include <dos.h>
#include <process.h>
#include <sys/timeb.h>
#include <sys/utime.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#if __MSVCRT_VERSION__ < 0x0700
   #define __MSVCRT_VERSION__ 0x0700    /* whatever above 0x0601 */
#endif
#include <time.h>
#include <sys/utime.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <algorithm>
#include <exception>
#include <vector>
#include <string>
#include <string.h>
#include <deque>
#include <map>
#include <unordered_map>
#include <chrono>
#define null NULL

// ------------------------------------------------------------------------

#ifdef EXTERNAL
   #undef EXTERNAL
#endif
#ifdef INIT
   #undef INIT
#endif

#ifdef __UTIL_CPP__  // Included from. Must be defined at read.cpp beginning
   #define EXTERNAL
   #define INIT(a) = a
#else
   #define EXTERNAL extern
   #define INIT(a)
#endif

//
// Researched other, *faster* random number generators further below.
//
void initrnd() { srand((unsigned)time(NULL)); }
int rnd(int n) { return(rand()%(n+1)); } //0-n

/* ===== defaults: =====
char *FmtYYYYMMDD_HH24MISS(char *pszBuff = NULL) */
char *FmtYYYYMMDD_HH24MISS(char *pszBuff)
   {
   tm tmNow;
   time_t ts = time(NULL);
   memcpy(&tmNow, localtime(&ts), sizeof(tm));
   tmNow.tm_year += 1900; // Since tm_year starts from 1900, only last 2 digits
   tmNow.tm_mon  += 1;    // Months since January, 0-11
    
   static char szBuff[16];
   if(!pszBuff) pszBuff = szBuff;
   snprintf(pszBuff, 16, "%04d%02d%02d_%02d%02d%02d", 
      tmNow.tm_year, tmNow.tm_mon, tmNow.tm_mday, 
      tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);
   return pszBuff;
   }

char *mstrrchr(char *start, char *middle, char c) //Reverse search for a character
   {
   if(middle==NULL) //then start from the end
      middle = start + strlen(start)-1;
   while(middle>=start)
      {
      if(*middle==c) return middle;
      middle--;
      }
   return NULL;
   }

char *rem1310(char *s)
   {   
   char *start = s;
   char *f=s;
   while(*f)
      {
      if(*f!=13 && *f!=10) { *s=*f; s++; }
      f++;
      }
   *s=0;
   return start;
   }

char *trimleft(char *str)
   {
   char *strstart = str;
   char *p = str;
   while(*p)
      {
      if(*p!=32 && *p!='\t') break;
      p++;
      }
   if(*p==0) return strstart;
   while(*p)
      {
      *str=*p;
      p++;
      str++;
      }
   *str=0;
   return strstart;
   }
   
char *trimright(char *str)
   {
   int i;
   for(i=strlen(str)-1; i>=0; --i)
      {
      if(str[i]!=' ' && str[i]!='\t') return str;
      str[i]=0;
      }
   return str;
   }

char *trim(char *str)
   {
   return trimleft(trimright(str));
   }
      
// ------------------------------------------------------------------------

class cmutex
   {
public:

#ifdef WINDOWS
   CRITICAL_SECTION stMutex;
   CRITICAL_SECTION *pstMutex;
#else
   pthread_mutex_t stMutex;
   pthread_mutex_t *pstMutex;
#endif

   void init()
      {
#ifdef WINDOWS
      InitializeCriticalSection(pstMutex);
#else
      pthread_mutexattr_t attr;
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_settype(&attr, /*PTHREAD_MUTEX_NORMAL*/ PTHREAD_MUTEX_RECURSIVE);
      pthread_mutex_init(pstMutex, &attr);
      pthread_mutexattr_destroy(&attr);
      // Just calling one of these makes locking work in signal handlers, not sure why yet.
      // pthread_create(NULL, NULL, NULL, NULL);
      pthread_getconcurrency();
#endif
      }

   cmutex()
      {
      pstMutex = &stMutex;
      }

   cmutex(bool bDoInit)
      {
      pstMutex = &stMutex;
      if(bDoInit) init();
      }
    
   virtual ~cmutex()
      {
      }

   void close()
      {
#ifdef WINDOWS
      DeleteCriticalSection(pstMutex);
#else
      pthread_mutex_unlock(pstMutex);
      pthread_mutex_destroy(pstMutex);
#endif
      }

   bool lock()
      {
#ifdef WINDOWS
      EnterCriticalSection(pstMutex);
      return true;
#else
      return pthread_mutex_lock(pstMutex) == 0;
#endif
      }

   bool trylock()
      {
#ifdef WINDOWS
      return TryEnterCriticalSection(pstMutex);
#else
      return pthread_mutex_trylock(pstMutex) == 0;
#endif
      }

   bool unlock()
      {
#ifdef WINDOWS
      LeaveCriticalSection(pstMutex);
      return true;
#else
      return pthread_mutex_unlock(pstMutex) == 0;
#endif
      }
   };

class cthread
   {
public:

#ifdef WINDOWS
   unsigned int uiThreadID;
   HANDLE       hThread;
   HANDLE       hEvent;
#else
   pthread_attr_t stThreadAttributes;
   pthread_t      stThreadID;
#endif

   cthread()
      {
      }

   virtual ~cthread()
      {
      // should we kill?
      // should we wait?
      // should we wait a little while, then kill?
      // I suppose this one is up to the parent thread to manage, lets stay out of it.
      }

#ifdef WINDOWS
   bool create(unsigned __stdcall pfunc(void *), unsigned int iStackSizeBytes)
#else
   bool create(void *pfunc(void *), unsigned int iStackSizeBytes)
#endif   
      {
#ifdef WINDOWS

      int ErrorNumber;
      int DOSErrorNumber;

      hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
      hThread = (HANDLE)_beginthreadex(NULL, iStackSizeBytes, pfunc, &hEvent, 0, &uiThreadID);
      if(hThread == 0 || !SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL)) //THREAD_PRIORITY_BELOW_NORMAL //THREAD_PRIORITY_NORMAL //THREAD_PRIORITY_ABOVE_NORMAL
         {
         ErrorNumber = errno;
         DOSErrorNumber = _doserrno;
         //cerr << "Begin thread error: " << strerror(ErrorNumber) << '\n';
         //cerr << "Begin thread DOS error code: " << DOSErrorNumber << '\n';
         return true;
         }
         
      return false;
#else
      //
      // Some Initializations, may only need to be done once
      //
      int status = pthread_attr_init(&stThreadAttributes);
      if(status != 0)
         {
         return true;
         }
      status = pthread_attr_setdetachstate(&stThreadAttributes, PTHREAD_CREATE_DETACHED);
      if(status != 0)
         {
         return true;
         }
      if(iStackSizeBytes)
         {
         status = pthread_attr_setstacksize(&stThreadAttributes, iStackSizeBytes);
         if (status != 0)
            {
            return true;
            }
         }
         
      //
      // Starting the thread
      //
      
      int iResult = pthread_create(&stThreadID, &stThreadAttributes, pfunc, NULL);
      if(iResult != 0)
         {
         return true;
         }

      pthread_attr_destroy(&stThreadAttributes);
      
      return false;
#endif
      };
      
   bool wait()
      {
#ifdef WINDOWS
      return WaitForSingleObject(hThread, INFINITE) != 0x0;
#else
      return pthread_join(stThreadID, NULL) != 0;
#endif
      }
      
   bool kill()
      {
#ifdef WINDOWS
      bool bResult = false;
      if(TerminateThread(hThread, 1) != 0x0) bResult = true;
      if(CloseHandle(hEvent)         != 0x0) bResult = true;
      if(CloseHandle(hThread)        != 0x0) bResult = true;
      return bResult;
#else
      return pthread_cancel(stThreadID) != 0;
#endif
      }      
   };

// ------------------------------------------------------------------------

EXTERNAL bool          _DBG_bSendToScreen      INIT(true);
EXTERNAL bool          _DBG_bSendToFile        INIT(false);
EXTERNAL bool          _DBG_flush              INIT(false); // Slower if true and not nice on IO, depends what we are trying to debug
EXTERNAL cmutex        _DBG_OutputMutex;
EXTERNAL FILE         *_DBG_fd                 INIT(NULL);
EXTERNAL int           _DBG_FileDay            INIT(0);
EXTERNAL char          _DBG_BaseFileName[2048] INIT({0});

EXTERNAL void CreateAccessable(char *pFile);
EXTERNAL void _DBG_Wait();
EXTERNAL void _DBG_Proceed();
EXTERNAL void _DBG_WithFile(char *szFile, int iLineNo, const char *szFunction);
EXTERNAL void _DBGS_WithFile(char *szFile, int iLineNo, const char *szFunction, const char *format, ... );
EXTERNAL void _DBG(char *szFile, int iLineNo, const char *szFunction);
EXTERNAL void _DBGS(char *szFile, int iLineNo, const char *szFunction, const char *format, ... );

#define DBG() { _DBG_WithFile(__FILE__, __LINE__, __FUNCTION__);}
#define DBGS(a, b...) {_DBGS_WithFile(__FILE__, __LINE__, __FUNCTION__, a, ## b);}

// ------------------------------------------------------------------------

#ifdef WINDOWS
//https://stackoverflow.com/questions/3059343/how-to-give-everybody-full-rights-to-a-file-programmatically
#include <Accctrl.h>
#include <Aclapi.h>
void SetFilePermission(LPCSTR FileName)
   {
   PSID pEveryoneSID = NULL;
   PACL pACL = NULL;
   EXPLICIT_ACCESS ea[1];
   SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;

   // Create a well-known SID for the Everyone group.
   if(!AllocateAndInitializeSid(&SIDAuthWorld, 1,
      SECURITY_WORLD_RID,
      0, 0, 0, 0, 0, 0, 0,
      &pEveryoneSID)) return;

   // Initialize an EXPLICIT_ACCESS structure for an ACE.
   ZeroMemory(&ea, 1 * sizeof(EXPLICIT_ACCESS));
   ea[0].grfAccessPermissions = 0xFFFFFFFF;
   ea[0].grfAccessMode = GRANT_ACCESS; //DENY_ACCESS;
   ea[0].grfInheritance= NO_INHERITANCE;
   ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
   ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
   ea[0].Trustee.ptstrName  = (LPTSTR) pEveryoneSID;

   // Create a new ACL that contains the new ACEs.
   if(SetEntriesInAclA(1, ea, NULL, &pACL) != ERROR_SUCCESS) 
      {
      FreeSid(pEveryoneSID);
      return;
      }

   // Initialize a security descriptor.  
   PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, 
      SECURITY_DESCRIPTOR_MIN_LENGTH); 
   if(!pSD)
      {
      FreeSid(pEveryoneSID);
      return;
      }

   if(!InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION)) 
      {
      FreeSid(pEveryoneSID);
      LocalFree(pACL);
      return;
      }

   // Add the ACL to the security descriptor. 
   if(!SetSecurityDescriptorDacl(pSD, 
      TRUE,     // bDaclPresent flag   
      pACL, 
      FALSE))   // not a default DACL 
      {
      FreeSid(pEveryoneSID);
      LocalFree(pACL);
      return;        
      }

   //Change the security attributes
   if(!SetFileSecurityA(FileName, DACL_SECURITY_INFORMATION, pSD)) 
      {
      FreeSid(pEveryoneSID);
      LocalFree(pACL);
      LocalFree(pSD);
      }

   FreeSid(pEveryoneSID);
   LocalFree(pACL);
   LocalFree(pSD);
   }
#endif

void CreateAccessable(char *pFile)
   {
#ifdef WINDOWS      
   FILE *fp = fopen(pFile, "a+b");
   if(fp) fclose(fp); // Yup, really have to do it after...   
   ///////////////////////////// still has popup
   //char szCmd[2048*2];
   //snprintf(szCmd, sizeof(szCmd), "cacls.exe \"%s\" /e /p Everyone:F", pFile);   
   //WinExec(szCmd, SW_HIDE);      
   ///////////////////////////// do following instead...   
   SetFilePermission(pFile);   
#endif
   //int fd=open(pFile, (mode_t)0777, "a+b"); // no effect, hmmm.
   //if(fd) close(fd);
   }

void _DBG_Wait()
   {
   static bool bMutexInit = false;   
   if(bMutexInit == false)
      {
      _DBG_OutputMutex.init();
      bMutexInit = true;
      }
   _DBG_OutputMutex.lock();
   }

void _DBG_Proceed()
   {
   _DBG_OutputMutex.unlock();
   }
      
void _DBG_WithFile(char *szFile, int iLineNo, const char *szFunction)
   {
   _DBG_Wait();
   tm stTime;
   time_t ts = time(NULL);
   memcpy(&stTime, localtime(&ts), sizeof(tm));
   stTime.tm_year += 1900; // Since tm_year starts from 1900, only last 2 digits
   stTime.tm_mon  += 1;    // Months since January, 0-11
   if(_DBG_bSendToScreen)
      {
      printf("%04d/%02d/%02d %02d:%02d:%02d %s: %d: %s:\n", 
         stTime.tm_year, stTime.tm_mon, stTime.tm_mday, 
         stTime.tm_hour, stTime.tm_min, stTime.tm_sec,
         szFile, iLineNo, szFunction);
      if(_DBG_flush) {fflush(stdout); fflush(stderr);}
      }
   if(!_DBG_bSendToFile)
      {
      _DBG_Proceed();
      return;
      }
   if(_DBG_fd && _DBG_FileDay != stTime.tm_mday)
      {
      fflush(_DBG_fd);
      fclose(_DBG_fd);
      _DBG_fd = NULL;
      }
   while(1)
      {
      if(!_DBG_fd)
         {
         _DBG_FileDay = stTime.tm_mday;
         char szFile[2048];
         snprintf(szFile, sizeof(szFile), "%s_%04d%02d%02d.log", 
            _DBG_BaseFileName, stTime.tm_year, stTime.tm_mon, stTime.tm_mday);
         CreateAccessable(szFile);
         if(!(_DBG_fd = fopen(szFile, "a+b")))
            {
            fprintf(stderr, "Could not open [%s] for write appaned.\n", szFile);
            exit(-1);
            }
         }
      fprintf(_DBG_fd, "%04d/%02d/%02d %02d:%02d:%02d %s: %d: %s:\n", 
         stTime.tm_year, stTime.tm_mon, stTime.tm_mday, 
         stTime.tm_hour, stTime.tm_min, stTime.tm_sec,       
         szFile, iLineNo, szFunction);
      if(ferror(_DBG_fd))
         {
         fprintf(stderr, "Could not fprintf the debug file.\n");
         fflush(_DBG_fd);
         fclose(_DBG_fd);
         _DBG_fd = NULL;
         }
      else break;
      }
   if(_DBG_flush) fflush(_DBG_fd); 
   _DBG_Proceed();
   }

void _DBGS_WithFile(char *szFile, int iLineNo, const char *szFunction, const char *format, ... )
   {
   static char buff[65536];
   _DBG_Wait();  
   tm stTime;
   time_t ts = time(NULL);
   memcpy(&stTime, localtime(&ts), sizeof(tm));
   stTime.tm_year += 1900; // Since tm_year starts from 1900, only last 2 digits
   stTime.tm_mon  += 1;    // Months since January, 0-11

   va_list argptr;
   va_start (argptr, format);
   vsnprintf((char*)buff, sizeof(buff), format, argptr);
   va_end (argptr);

   if(_DBG_bSendToScreen)
      {
      printf("%04d/%02d/%02d %02d:%02d:%02d %s: %d: %s: %s\n", 
         stTime.tm_year, stTime.tm_mon, stTime.tm_mday, 
         stTime.tm_hour, stTime.tm_min, stTime.tm_sec,
         szFile, iLineNo, szFunction, buff);
      if(_DBG_flush) {fflush(stdout); fflush(stderr);}
      }
   if(!_DBG_bSendToFile)
      {
      _DBG_Proceed();
      return;
      }
   if(_DBG_fd && _DBG_FileDay != stTime.tm_mday)
      {
      fflush(_DBG_fd);
      fclose(_DBG_fd);
      _DBG_fd = NULL;
      }
   while(1)
      {
      if(!_DBG_fd)
         {
         _DBG_FileDay = stTime.tm_mday;
         char szFile[2048];
         snprintf(szFile, sizeof(szFile), "%s_%04d%02d%02d.log", 
            _DBG_BaseFileName, stTime.tm_year, stTime.tm_mon, stTime.tm_mday);
         CreateAccessable(szFile);
         if(!(_DBG_fd = fopen(szFile, "a+b")))
            {
            fprintf(stderr, "Could not open [%s] for write appaned.\n", szFile);
            exit(-1);
            }
         }
      fprintf(_DBG_fd, "%04d/%02d/%02d %02d:%02d:%02d %s: %d: %s: %s\n", 
         stTime.tm_year, stTime.tm_mon, stTime.tm_mday, 
         stTime.tm_hour, stTime.tm_min, stTime.tm_sec,       
         szFile, iLineNo, szFunction, buff);
      if(ferror(_DBG_fd))
         {
         fprintf(stderr, "Could not fprintf the debug file.\n");
         fflush(_DBG_fd);
         fclose(_DBG_fd);
         _DBG_fd = NULL;
         }
      else break;
      }
   if(_DBG_flush) fflush(_DBG_fd);  
   _DBG_Proceed();
   }

// ------------------------------------------------------------------------

typedef struct mstrtok_typ
   { 
   char *mstrtok(const char *pszInStr = NULL, const char *pszInTokens = NULL);   
   char *mstrrtok(const char *pszInStr = NULL, const char *pszInTokens = NULL);
   void  close();
   char  cToken;     //THE RIGHT SIDE FOUND TOKEN
   char *pToken;
   char  cTokenPrev; //THE LEFT SIDE FOUND TOKEN 
   char *pTokenPrev;
   //------ Private -------
   char *pszMemInTokens_1;    //mstrtok
   char *pszMemInTokens_2;    //mstrrtok
   char *pszCurrInStrPos_2;   //mstrrtok
   char *os_2;                //mstrrtok
   } mstrtok;
   
// ------------------------------------------------------------------------
//
//
//  EXAMPLE:  
//
//     char szBuff[100] = {0x0}
//     sprintf(szBuff, " $% Hello, World. Example  " );
//     
//     mstrtok m0;      
//     for(char *szPtr = m0.mstrtok( szBuff, " $%,."); szPtr!=NULL; szPtr=m0.mstrtok())
//     {
//          printf("\"%s\"\n",szPtr);
//
//     }  // When complete... the original szBuff is unchanged
//
//
//   OUTPUT:                USING mstrrtok:
//
//     "Hello"                  "Example"
//     "World"                  "World"
//     "Example"                "Hello"
//
//
//  OTHER:
//
//    m0.cToken = the token to the right of the returned string
//    m0.cTokenPrev = the token to the left of the returned string
// 
//  NOTE: This was optimized almost to assembly, there is a newer version 
//        but the additinal features really arn't needed for this applciation.
//
//
// ------------------------------------------------------------------------

//typedef struct mstrtok_typ
//   { 
//   char  cToken;     //THE RIGHT SIDE FOUND TOKEN
//   char *pToken;
//   char  cTokenPrev; //THE LEFT SIDE FOUND TOKEN 
//   char *pTokenPrev;
//      ...
//   } mstrtok;
  

/* === Default ===
char *mstrtok::mstrtok(const char *pszInStr = NULL, const char *pszInTokens = NULL) */
char *mstrtok::mstrtok(const char *pszInStr, const char *pszInTokens)
   {  
   char *pszReturnStr;

   if(pszInStr==NULL)
      {           
      if(pToken == NULL) return NULL;
      *pToken = cToken;
      while(*pToken)
         {
         pTokenPrev = pszMemInTokens_1;
         while(*pTokenPrev)
            {
            if(*pToken==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev==0) break;
         else cTokenPrev = *pTokenPrev; 
         pToken++;
         }
      if(*pToken==0) 
         {
         pToken = NULL;
         return NULL;
         }
      pszReturnStr = pToken;
      while(*pToken)
         {
         pTokenPrev = pszMemInTokens_1;
         while(*pTokenPrev)
            {
            if(*pToken==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev!=0) break;
         pToken++;
         }
      if(*pToken==0) 
         {
         pToken = NULL;
         cToken = 0;
         return pszReturnStr;
         }
      if(*pTokenPrev!=0)
         {
         cToken = *pToken;
         *pToken=0;
         return pszReturnStr;
         }      
      }
   else
      {
      pToken = (char*)pszInStr;
      pszMemInTokens_1 = (char*)pszInTokens;
      while(*pToken)
         {
         pTokenPrev = pszMemInTokens_1;
         while(*pTokenPrev)
            {
            if(*pToken==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev==0) break;
         else cTokenPrev = *pTokenPrev;
         pToken++;
         }
      if(*pToken==0) 
         {
         pToken = NULL;
         return NULL;
         }
      pszReturnStr = pToken;
      while(*pToken)
         {
         pTokenPrev = pszMemInTokens_1;
         while(*pTokenPrev)
            {
            if(*pToken==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev!=0) break;
         pToken++;
         }
      if(*pToken==0) 
         {
         pToken = NULL;
         cToken = 0;
         return pszReturnStr;
         }
      if(*pTokenPrev!=0)
         {
         cToken = *pToken;
         *pToken=0;
         return pszReturnStr;
         }
      }
   return NULL; //shouldnt happen
   }
      
/* === Default ===
char *mstrtok::mstrrtok(const char *pszInStr = NULL, const char *pszInTokens = NULL) */
char *mstrtok::mstrrtok(const char *pszInStr, const char *pszInTokens)
   {     
   char *pszReturnStr;

   if(pszInStr==NULL)
      {
      *pToken = cToken;
      if(pszCurrInStrPos_2 == NULL) return NULL;      
         cTokenPrev = 0;
      while(pszCurrInStrPos_2!=os_2)
         {
         pTokenPrev = pszMemInTokens_2;
         while(*pTokenPrev)
            {
            if(*pszCurrInStrPos_2==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev==0)
            {
            pToken = pszCurrInStrPos_2+1;
            cToken = *pToken;            
            *(pszCurrInStrPos_2+1)=0;
            break;
            }
         pszCurrInStrPos_2--;
         }
      if(pszCurrInStrPos_2==os_2) 
         {
         pszCurrInStrPos_2 = NULL;
         return NULL;
         }
      while(pszCurrInStrPos_2!=os_2)
         {
         pTokenPrev = pszMemInTokens_2;
         while(*pTokenPrev)
            {
            if(*pszCurrInStrPos_2==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev!=0) break;
         pszCurrInStrPos_2--;
         }
      pszReturnStr = pszCurrInStrPos_2+1;
      if(pszCurrInStrPos_2==os_2)
         pszCurrInStrPos_2 = NULL;
      else
         cTokenPrev = *pTokenPrev;
      return pszReturnStr;
      }
   else
      {
      pszCurrInStrPos_2 = (char*)pszInStr+strlen(pszInStr)-1;
      os_2 = (char*)pszInStr-1;
      pszMemInTokens_2 = (char*)pszInTokens;
      cTokenPrev = 0;
      while(pszCurrInStrPos_2!=os_2)
         {
         pTokenPrev = pszMemInTokens_2;
         while(*pTokenPrev)
            {
            if(*pszCurrInStrPos_2==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev==0)
            {
            pToken = pszCurrInStrPos_2+1;
            cToken = *pToken;
            *pToken=0;
            break;
            }
         pszCurrInStrPos_2--;
         }
      if(pszCurrInStrPos_2==os_2)
         {
         pszCurrInStrPos_2 = NULL;
         return NULL;
         }
      while(pszCurrInStrPos_2!=os_2)
         {
         pTokenPrev = pszMemInTokens_2;
         while(*pTokenPrev)
            {
            if(*pszCurrInStrPos_2==*pTokenPrev) break;
            pTokenPrev++;
            }
         if(*pTokenPrev!=0) break;
         pszCurrInStrPos_2--;
         }
      pszReturnStr = pszCurrInStrPos_2+1;
      if(pszCurrInStrPos_2==os_2)
         pszCurrInStrPos_2 = NULL;
      else
         cTokenPrev = *pTokenPrev;              
      return pszReturnStr;
      }
   return NULL; //shouldnt happen
   }
   
void mstrtok::close()
   {
   if(pToken) *pToken = cToken;   
   }

// ------------------------------------------------------------------------

char szArgv0[2048] = {0x0};
   // note: test.exe will come back 'test' or 'test.exe' depending how its called (with .exe or not). argv[0] is so annoying. Even full path if clicked on.
   // lets fix it and remove the .exe if it is there.
#define setArgv0()                                                                                           \
   if(strlen(argv[0]))                                                                                       \
      {                                                                                                      \
      if(strchr(argv[0], '\\'))                                                                              \
         snprintf(szArgv0, sizeof(szArgv0), "%s", mstrrchr(argv[0], &argv[0][strlen(argv[0])-1], '\\')+1);   \
      else if(strchr(argv[0], '/'))                                                                          \
         snprintf(szArgv0, sizeof(szArgv0), "%s", mstrrchr(argv[0], &argv[0][strlen(argv[0])-1], '/')+1);    \
      else                                                                                                   \
         snprintf(szArgv0, sizeof(szArgv0), "%s", argv[0]);                                                  \
      if(strlen(szArgv0))                                                                                    \
         {                                                                                                   \
         char *pPtr = mstrrchr(szArgv0, &szArgv0[strlen(szArgv0)-1], '.');                                   \
         if(pPtr && strcasecmp(pPtr, ".exe")==0)                                                             \
            *pPtr = 0;                                                                                       \
         }                                                                                                   \
      }

// ------------------------------------------------------------------------



bool bShutdown = false;
void MySIG(int x)
   {
   bShutdown = true;
   //printf("Hello From Handler\n");
   }

//
// Some random number generators, I need performance, not cryptogrphically sound.
// Lets save them, might be nice to play with later.
//

//
//
// Faster but less safe random number generators
//

static unsigned long rnd3_x=123456789;
static unsigned long rnd3_y=362436069;
static unsigned long rnd3_z=521288629;
unsigned long rnd3(unsigned long n) //xorshf96(void) 
   {          
   //period 2^96-1
   unsigned long t;
   rnd3_x ^= rnd3_x << 16;
   rnd3_x ^= rnd3_x >> 5;
   rnd3_x ^= rnd3_x << 1;
   t = rnd3_x;
   rnd3_x = rnd3_y;
   rnd3_y = rnd3_z;
   rnd3_z = t ^ rnd3_x ^ rnd3_y;
   return rnd3_z % (n+1);
   }

inline unsigned int rnd4(unsigned int n)
   { 
   static unsigned int g_seed = time(NULL);
   g_seed = (214013*g_seed+2531011); 
   return ((g_seed>>16)&0x7FFF)%(n+1);
   }

//
// This one is off an Intel website. Interesting...
//
#ifndef RAND_SSE_H
#define RAND_SSE_H
#endif
#include "emmintrin.h"
#define COMPATABILITY
//define this if you wish to return values similar to the standard rand();
void srand_sse(unsigned int seed);
void rand_sse(unsigned int *);
void initrnd5() { srand_sse((unsigned)time(NULL)); }
unsigned int rnd5(unsigned int n) 
   { 
   unsigned int iResult;
   rand_sse(&iResult); //0-n
   return iResult%(n+1);
   }
__declspec( align(16) ) static __m128i cur_seed;
void srand_sse(unsigned int seed)
   { cur_seed = _mm_set_epi32(seed, seed+1, seed, seed+1); }
inline void rand_sse(unsigned int *result)
   {
   __declspec( align(16) ) __m128i cur_seed_split;
   __declspec( align(16) ) __m128i multiplier;
   __declspec( align(16) ) __m128i adder;
   __declspec( align(16) ) __m128i mod_mask;
   __declspec( align(16) ) __m128i sra_mask;
   __declspec( align(16) ) __m128i sseresult;
   __declspec( align(16) ) static const unsigned int mult[4] =
      { 214013, 17405, 214013, 69069 };
   __declspec( align(16) ) static const unsigned int gadd[4] =
      { 2531011, 10395331, 13737667, 1 };
   __declspec( align(16) ) static const unsigned int mask[4] =
      { 0xFFFFFFFF, 0, 0xFFFFFFFF, 0 };
   __declspec( align(16) ) static const unsigned int masklo[4] =
      { 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF };
   adder = _mm_load_si128( (__m128i*) gadd);
   multiplier = _mm_load_si128( (__m128i*) mult);
   mod_mask = _mm_load_si128( (__m128i*) mask);
   sra_mask = _mm_load_si128( (__m128i*) masklo);
   cur_seed_split = _mm_shuffle_epi32( cur_seed, _MM_SHUFFLE( 2, 3, 0, 1 ) );
   cur_seed = _mm_mul_epu32( cur_seed, multiplier );
   multiplier = _mm_shuffle_epi32( multiplier, _MM_SHUFFLE( 2, 3, 0, 1 ) );
   cur_seed_split = _mm_mul_epu32( cur_seed_split, multiplier );
   cur_seed = _mm_and_si128( cur_seed, mod_mask);
   cur_seed_split = _mm_and_si128( cur_seed_split, mod_mask );
   cur_seed_split = _mm_shuffle_epi32( cur_seed_split, _MM_SHUFFLE( 2, 3, 0, 1 ) );
   cur_seed = _mm_or_si128( cur_seed, cur_seed_split );
   cur_seed = _mm_add_epi32( cur_seed, adder);
   #ifdef COMPATABILITY
   // Add the lines below if you wish to reduce your results to 16-bit vals...
   sseresult = _mm_srai_epi32( cur_seed, 16);
   sseresult = _mm_and_si128( sseresult, sra_mask );
   _mm_storeu_si128( (__m128i*) result, sseresult );
   return;
   #endif
   _mm_storeu_si128( (__m128i*) result, cur_seed);
   return;
   }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// OLL recognition for the application. 
//    Note: 2 of them I do at a different angle than the standard.
//          You can change this if you'de like to any orientation you want.
//
char *aOLL[57] = // 9*5*1 size for each
   {
   "    y    " //1
   "y       y"
   "y   y   y"
   "y       y"
   "    y    "
   ,
   "    y y  " //2
   "y        "
   "y   y   y"
   "y        "
   "    y y  "
   ,
   "  y y    " //3
   "        y"
   "y   y   y"
   "y     y  "
   "    y    "
   ,
   "    y    " //4
   "y     y  "
   "y   y   y"
   "        y"
   "  y y    "
   ,
   "  y y    " //5
   "        y"
   "y   y y  "
   "y   y y  "
   "         "
   ,
   "         " //6
   "y   y y  "
   "y   y y  "
   "        y"
   "  y y    "
   ,
   "  y      " //7
   "    y   y"
   "  y y   y"
   "  y      "
   "    y y  "
   ,
   "    y y  " //8
   "  y      "
   "  y y   y"
   "    y   y"
   "  y      "
   ,
   "      y  " //9
   "y   y    "
   "  y y   y"
   "      y  "
   "  y y    "
   ,
   "  y y    " //10
   "      y  "
   "  y y   y"
   "y   y    "
   "      y  "
   ,
   "  y y    " //11
   "        y"
   "y   y y  "
   "  y y    "
   "      y  "
   ,
   "      y  " //12
   "  y y    "
   "y   y y  "
   "        y"
   "  y y    "
   ,
   "  y y    " //13
   "      y  "
   "  y y y  "
   "y        "
   "    y y  "
   ,
   "    y y  " //14
   "y        "
   "  y y y  "
   "      y  "
   "  y y    "
   ,
   "  y y    " //15
   "        y"
   "  y y y  "
   "y     y  "
   "    y    "
   ,
   "    y    " //16
   "y     y  "
   "  y y y  "
   "        y"
   "  y y    "
   ,
   "    y y  " //17
   "  y      "
   "y   y   y"
   "y     y  "
   "    y    "
   ,
   "    y    " //18
   "  y   y  "
   "y   y   y"
   "         "
   "  y y y  "
   ,
   "    y    " //19
   "  y   y  "
   "y   y   y"
   "y       y"
   "    y    "
   ,
   "    y    " //20
   "  y   y  "
   "y   y   y"
   "  y   y  "
   "    y    "
   ,
   "         " //21
   "y   y   y"
   "  y y y  "
   "y   y   y"
   "         "
   ,
   "      y  " //22
   "y   y    "
   "  y y y  "
   "y   y    "
   "      y  "
   ,
   "  y   y  " //23
   "    y    "
   "  y y y  "
   "  y y y  "
   "         "
   ,
   "         " //24
   "y   y   y"
   "  y y y  "
   "  y y y  "
   "         "
   ,
   "  y      " //25
   "    y y  "
   "  y y y  "
   "  y y   y"
   "         "
   ,
   "      y  " //26
   "  y y    "
   "  y y y  "
   "    y   y"
   "  y      "
   ,
   "  y      " //27
   "    y y  "
   "  y y y  "
   "y   y    "
   "      y  "
   ,
   "    y    " //28
   "  y   y  "
   "  y y   y"
   "  y y y  "
   "         "
   ,
   "    y    " //29
   "  y   y  "
   "  y y   y"
   "y   y   y"
   "         "
   ,
   "    y    " //30
   "  y   y  "
   "y   y y  "
   "y   y   y"
   "         "
   ,
   "    y y  " //31
   "  y      "
   "  y y   y"
   "  y y    "
   "      y  "
   ,
   "  y y    " //32
   "      y  "
   "y   y y  "
   "    y y  "
   "  y      "
   ,
   "  y y    " //33
   "      y  "
   "  y y y  "
   "      y  "
   "  y y    "
   ,
   "    y    " //34
   "y       y"
   "  y y y  "
   "  y   y  "
   "    y    "
   ,
   "    y    " //35
   "  y     y"
   "y   y y  "
   "    y y  "
   "  y      "
   ,
   "    y    " //36
   "  y     y"
   "  y y   y"
   "    y y  "
   "  y      "
   ,
   "         " //37
   "  y y   y"
   "  y y   y"
   "      y  "
   "  y y    "
   ,
   "  y      " //38
   "    y y  "
   "  y y   y"
   "  y     y"
   "    y    "
   ,
   "    y    " //39
   "y     y  "
   "  y y y  "
   "  y      "
   "    y y  "
   ,
   "    y y  " //40
   "  y      "
   "  y y y  "
   "y     y  "
   "    y    "
   ,
   "    y    " //41
   "  y   y  "
   "y   y y  "
   "    y    "
   "  y   y  "
   ,
   "    y    " //42
   "  y   y  "
   "  y y   y"
   "    y    "
   "  y   y  "
   ,
   "    y    " //43
   "  y     y"
   "  y y   y"
   "  y y   y"
   "         "
   ,
   "    y    " //44
   "y     y  "
   "y   y y  "
   "y   y y  "
   "         "
   ,
   "    y    " //45
   "y     y  "
   "  y y y  "
   "y     y  "
   "    y    "
   ,
   "         " //46
   "  y y   y"
   "y   y   y"
   "  y y   y"
   "         "
   ,
   "  y      " //47
   "    y   y"
   "y   y y  "
   "        y"
   "  y y    "
   ,
   "      y  " //48
   "y   y    "
   "  y y   y"
   "y        "
   "    y y  "
   ,
   "  y y    " //49
   "        y"
   "  y y   y"
   "    y   y"
   "  y      "
   ,
   "    y y  " //50
   "y        "
   "y   y y  "
   "y   y    "
   "      y  "
   ,
   "    y y  " //51
   "y        "
   "  y y y  "
   "y        "
   "    y y  "
   ,
   "      y  " //52
   "y   y    "
   "y   y   y"
   "y   y    "
   "      y  "
   ,
   "  y y y  " //53
   "         "
   "  y y   y"
   "    y    "
   "  y   y  "
   ,
   "  y   y  " //54
   "    y    "
   "  y y   y"
   "         "
   "  y y y  "
   ,
   "  y y y  " //55
   "         "
   "  y y y  "
   "         "
   "  y y y  "
   ,
   "    y    " //56
   "y       y"
   "  y y y  "
   "y       y"
   "    y    "
   ,
   "    y    " //57
   "  y   y  "
   "  y y y  "
   "  y   y  "
   "    y    ",
   };

std::map<std::string, int> mOLL;

char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";

inline char *rotate(register char *pState, char cCmd, register char *pOutState)
   {
         //char aState[6*3*3] = // its not 3x3x3, its... um... each face perhaps... a 3x3 per face so 
         //{
         //'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', F 0 
         //'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', B 1
         //'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', L 2
         //'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', R 3
         //'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', U 4
         //'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y'  D 5
         //};
   memcpy(pOutState, pState, 6*3*3);
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2 
   //                    M  M' M2 f  f' f2 b  b' b2 l  l' l2 r  r' r2 u  u' u2 d  d' d2";
   switch(cCmd)
      {
         case 0: // F
            // face colors
            pOutState[0*3*3+ 0*3+0] = pState[0*3*3+ 2*3+0];
            pOutState[0*3*3+ 0*3+1] = pState[0*3*3+ 1*3+0];
            pOutState[0*3*3+ 0*3+2] = pState[0*3*3+ 0*3+0];

            pOutState[0*3*3+ 1*3+0] = pState[0*3*3+ 2*3+1];
            pOutState[0*3*3+ 1*3+1] = pState[0*3*3+ 1*3+1];
            pOutState[0*3*3+ 1*3+2] = pState[0*3*3+ 0*3+1];
            
            pOutState[0*3*3+ 2*3+0] = pState[0*3*3+ 2*3+2];
            pOutState[0*3*3+ 2*3+1] = pState[0*3*3+ 1*3+2];
            pOutState[0*3*3+ 2*3+2] = pState[0*3*3+ 0*3+2];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 2*3+0] = pState[2*3*3+ 2*3+2];
            pOutState[4*3*3+ 2*3+1] = pState[2*3*3+ 1*3+2];
            pOutState[4*3*3+ 2*3+2] = pState[2*3*3+ 0*3+2];
            
            pOutState[5*3*3+ 0*3+0] = pState[3*3*3+ 2*3+0];
            pOutState[5*3*3+ 0*3+1] = pState[3*3*3+ 1*3+0];
            pOutState[5*3*3+ 0*3+2] = pState[3*3*3+ 0*3+0];

            pOutState[2*3*3+ 0*3+2] = pState[5*3*3+ 0*3+0];
            pOutState[2*3*3+ 1*3+2] = pState[5*3*3+ 0*3+1];
            pOutState[2*3*3+ 2*3+2] = pState[5*3*3+ 0*3+2];

            pOutState[3*3*3+ 0*3+0] = pState[4*3*3+ 2*3+0];
            pOutState[3*3*3+ 1*3+0] = pState[4*3*3+ 2*3+1];
            pOutState[3*3*3+ 2*3+0] = pState[4*3*3+ 2*3+2];
            break;
         case 1: // F'
            // face colors
            pOutState[0*3*3+ 0*3+0] = pState[0*3*3+ 0*3+2];
            pOutState[0*3*3+ 0*3+1] = pState[0*3*3+ 1*3+2];
            pOutState[0*3*3+ 0*3+2] = pState[0*3*3+ 2*3+2];

            pOutState[0*3*3+ 1*3+0] = pState[0*3*3+ 0*3+1];
            pOutState[0*3*3+ 1*3+1] = pState[0*3*3+ 1*3+1];
            pOutState[0*3*3+ 1*3+2] = pState[0*3*3+ 2*3+1];
            
            pOutState[0*3*3+ 2*3+0] = pState[0*3*3+ 0*3+0];
            pOutState[0*3*3+ 2*3+1] = pState[0*3*3+ 1*3+0];
            pOutState[0*3*3+ 2*3+2] = pState[0*3*3+ 2*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 2*3+0] = pState[3*3*3+ 0*3+0];
            pOutState[4*3*3+ 2*3+1] = pState[3*3*3+ 1*3+0];
            pOutState[4*3*3+ 2*3+2] = pState[3*3*3+ 2*3+0];
            
            pOutState[5*3*3+ 0*3+0] = pState[2*3*3+ 0*3+2];
            pOutState[5*3*3+ 0*3+1] = pState[2*3*3+ 1*3+2];
            pOutState[5*3*3+ 0*3+2] = pState[2*3*3+ 2*3+2];

            pOutState[2*3*3+ 0*3+2] = pState[4*3*3+ 2*3+2];
            pOutState[2*3*3+ 1*3+2] = pState[4*3*3+ 2*3+1];
            pOutState[2*3*3+ 2*3+2] = pState[4*3*3+ 2*3+0];

            pOutState[3*3*3+ 0*3+0] = pState[5*3*3+ 0*3+2];
            pOutState[3*3*3+ 1*3+0] = pState[5*3*3+ 0*3+1];
            pOutState[3*3*3+ 2*3+0] = pState[5*3*3+ 0*3+0];
            break;
         case 2: // F2
            // face colors
            pOutState[0*3*3+ 0*3+0] = pState[0*3*3+ 2*3+2];
            pOutState[0*3*3+ 0*3+1] = pState[0*3*3+ 2*3+1];
            pOutState[0*3*3+ 0*3+2] = pState[0*3*3+ 2*3+0];

            pOutState[0*3*3+ 1*3+0] = pState[0*3*3+ 1*3+2];
            pOutState[0*3*3+ 1*3+1] = pState[0*3*3+ 1*3+1];
            pOutState[0*3*3+ 1*3+2] = pState[0*3*3+ 1*3+0];
            
            pOutState[0*3*3+ 2*3+0] = pState[0*3*3+ 0*3+2];
            pOutState[0*3*3+ 2*3+1] = pState[0*3*3+ 0*3+1];
            pOutState[0*3*3+ 2*3+2] = pState[0*3*3+ 0*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 2*3+0] = pState[5*3*3+ 0*3+2];
            pOutState[4*3*3+ 2*3+1] = pState[5*3*3+ 0*3+1];
            pOutState[4*3*3+ 2*3+2] = pState[5*3*3+ 0*3+0];
            
            pOutState[5*3*3+ 0*3+0] = pState[4*3*3+ 2*3+2];
            pOutState[5*3*3+ 0*3+1] = pState[4*3*3+ 2*3+1];
            pOutState[5*3*3+ 0*3+2] = pState[4*3*3+ 2*3+0];

            pOutState[2*3*3+ 0*3+2] = pState[3*3*3+ 2*3+0];
            pOutState[2*3*3+ 1*3+2] = pState[3*3*3+ 1*3+0];
            pOutState[2*3*3+ 2*3+2] = pState[3*3*3+ 0*3+0];

            pOutState[3*3*3+ 0*3+0] = pState[2*3*3+ 2*3+2];
            pOutState[3*3*3+ 1*3+0] = pState[2*3*3+ 1*3+2];
            pOutState[3*3*3+ 2*3+0] = pState[2*3*3+ 0*3+2];
            break;
         case 3: // B
            // face colors
            pOutState[1*3*3+ 0*3+0] = pState[1*3*3+ 2*3+0];
            pOutState[1*3*3+ 0*3+1] = pState[1*3*3+ 1*3+0];
            pOutState[1*3*3+ 0*3+2] = pState[1*3*3+ 0*3+0];

            pOutState[1*3*3+ 1*3+0] = pState[1*3*3+ 2*3+1];
            pOutState[1*3*3+ 1*3+1] = pState[1*3*3+ 1*3+1];
            pOutState[1*3*3+ 1*3+2] = pState[1*3*3+ 0*3+1];
            
            pOutState[1*3*3+ 2*3+0] = pState[1*3*3+ 2*3+2];
            pOutState[1*3*3+ 2*3+1] = pState[1*3*3+ 1*3+2];
            pOutState[1*3*3+ 2*3+2] = pState[1*3*3+ 0*3+2];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+0] = pState[3*3*3+ 0*3+2];
            pOutState[4*3*3+ 0*3+1] = pState[3*3*3+ 1*3+2];
            pOutState[4*3*3+ 0*3+2] = pState[3*3*3+ 2*3+2];
            
            pOutState[5*3*3+ 2*3+0] = pState[2*3*3+ 0*3+0];
            pOutState[5*3*3+ 2*3+1] = pState[2*3*3+ 1*3+0];
            pOutState[5*3*3+ 2*3+2] = pState[2*3*3+ 2*3+0];

            pOutState[2*3*3+ 0*3+0] = pState[4*3*3+ 0*3+2];
            pOutState[2*3*3+ 1*3+0] = pState[4*3*3+ 0*3+1];
            pOutState[2*3*3+ 2*3+0] = pState[4*3*3+ 0*3+0];

            pOutState[3*3*3+ 0*3+2] = pState[5*3*3+ 2*3+2];
            pOutState[3*3*3+ 1*3+2] = pState[5*3*3+ 2*3+1];
            pOutState[3*3*3+ 2*3+2] = pState[5*3*3+ 2*3+0];
            break;
         case 4: // B'
            pOutState[1*3*3+ 0*3+0] = pState[1*3*3+ 0*3+2];
            pOutState[1*3*3+ 0*3+1] = pState[1*3*3+ 1*3+2];
            pOutState[1*3*3+ 0*3+2] = pState[1*3*3+ 2*3+2];

            pOutState[1*3*3+ 1*3+0] = pState[1*3*3+ 0*3+1];
            pOutState[1*3*3+ 1*3+1] = pState[1*3*3+ 1*3+1];
            pOutState[1*3*3+ 1*3+2] = pState[1*3*3+ 2*3+1];
            
            pOutState[1*3*3+ 2*3+0] = pState[1*3*3+ 0*3+0];
            pOutState[1*3*3+ 2*3+1] = pState[1*3*3+ 1*3+0];
            pOutState[1*3*3+ 2*3+2] = pState[1*3*3+ 2*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+0] = pState[2*3*3+ 2*3+0];
            pOutState[4*3*3+ 0*3+1] = pState[2*3*3+ 1*3+0];
            pOutState[4*3*3+ 0*3+2] = pState[2*3*3+ 0*3+0];
            
            pOutState[5*3*3+ 2*3+0] = pState[3*3*3+ 2*3+2];
            pOutState[5*3*3+ 2*3+1] = pState[3*3*3+ 1*3+2];
            pOutState[5*3*3+ 2*3+2] = pState[3*3*3+ 0*3+2];

            pOutState[2*3*3+ 0*3+0] = pState[5*3*3+ 2*3+0];
            pOutState[2*3*3+ 1*3+0] = pState[5*3*3+ 2*3+1];
            pOutState[2*3*3+ 2*3+0] = pState[5*3*3+ 2*3+2];

            pOutState[3*3*3+ 0*3+2] = pState[4*3*3+ 0*3+0];
            pOutState[3*3*3+ 1*3+2] = pState[4*3*3+ 0*3+1];
            pOutState[3*3*3+ 2*3+2] = pState[4*3*3+ 0*3+2];
            break;
         case 5: // B2
            // face colors
            pOutState[1*3*3+ 0*3+0] = pState[1*3*3+ 2*3+2];
            pOutState[1*3*3+ 0*3+1] = pState[1*3*3+ 2*3+1];
            pOutState[1*3*3+ 0*3+2] = pState[1*3*3+ 2*3+0];

            pOutState[1*3*3+ 1*3+0] = pState[1*3*3+ 1*3+2];
            pOutState[1*3*3+ 1*3+1] = pState[1*3*3+ 1*3+1];
            pOutState[1*3*3+ 1*3+2] = pState[1*3*3+ 1*3+0];
            
            pOutState[1*3*3+ 2*3+0] = pState[1*3*3+ 0*3+2];
            pOutState[1*3*3+ 2*3+1] = pState[1*3*3+ 0*3+1];
            pOutState[1*3*3+ 2*3+2] = pState[1*3*3+ 0*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+0] = pState[5*3*3+ 2*3+2];
            pOutState[4*3*3+ 0*3+1] = pState[5*3*3+ 2*3+1];
            pOutState[4*3*3+ 0*3+2] = pState[5*3*3+ 2*3+0];
            
            pOutState[5*3*3+ 2*3+0] = pState[4*3*3+ 0*3+2];
            pOutState[5*3*3+ 2*3+1] = pState[4*3*3+ 0*3+1];
            pOutState[5*3*3+ 2*3+2] = pState[4*3*3+ 0*3+0];

            pOutState[2*3*3+ 0*3+0] = pState[3*3*3+ 2*3+2];
            pOutState[2*3*3+ 1*3+0] = pState[3*3*3+ 1*3+2];
            pOutState[2*3*3+ 2*3+0] = pState[3*3*3+ 0*3+2];

            pOutState[3*3*3+ 0*3+2] = pState[2*3*3+ 2*3+0];
            pOutState[3*3*3+ 1*3+2] = pState[2*3*3+ 1*3+0];
            pOutState[3*3*3+ 2*3+2] = pState[2*3*3+ 0*3+0];
            break;
         case 6: // L
            // face colors
            pOutState[2*3*3+ 0*3+0] = pState[2*3*3+ 2*3+0];
            pOutState[2*3*3+ 0*3+1] = pState[2*3*3+ 1*3+0];
            pOutState[2*3*3+ 0*3+2] = pState[2*3*3+ 0*3+0];

            pOutState[2*3*3+ 1*3+0] = pState[2*3*3+ 2*3+1];
            pOutState[2*3*3+ 1*3+1] = pState[2*3*3+ 1*3+1];
            pOutState[2*3*3+ 1*3+2] = pState[2*3*3+ 0*3+1];
            
            pOutState[2*3*3+ 2*3+0] = pState[2*3*3+ 2*3+2];
            pOutState[2*3*3+ 2*3+1] = pState[2*3*3+ 1*3+2];
            pOutState[2*3*3+ 2*3+2] = pState[2*3*3+ 0*3+2];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+0] = pState[1*3*3+ 2*3+2];
            pOutState[4*3*3+ 1*3+0] = pState[1*3*3+ 1*3+2];
            pOutState[4*3*3+ 2*3+0] = pState[1*3*3+ 0*3+2];
            
            pOutState[5*3*3+ 0*3+0] = pState[0*3*3+ 0*3+0];
            pOutState[5*3*3+ 1*3+0] = pState[0*3*3+ 1*3+0];
            pOutState[5*3*3+ 2*3+0] = pState[0*3*3+ 2*3+0];

            pOutState[1*3*3+ 0*3+2] = pState[5*3*3+ 2*3+0];
            pOutState[1*3*3+ 1*3+2] = pState[5*3*3+ 1*3+0];
            pOutState[1*3*3+ 2*3+2] = pState[5*3*3+ 0*3+0];

            pOutState[0*3*3+ 0*3+0] = pState[4*3*3+ 0*3+0];
            pOutState[0*3*3+ 1*3+0] = pState[4*3*3+ 1*3+0];
            pOutState[0*3*3+ 2*3+0] = pState[4*3*3+ 2*3+0];
            break;
         case 7: // L'
            // face colors
            pOutState[2*3*3+ 0*3+0] = pState[2*3*3+ 0*3+2];
            pOutState[2*3*3+ 0*3+1] = pState[2*3*3+ 1*3+2];
            pOutState[2*3*3+ 0*3+2] = pState[2*3*3+ 2*3+2];

            pOutState[2*3*3+ 1*3+0] = pState[2*3*3+ 0*3+1];
            pOutState[2*3*3+ 1*3+1] = pState[2*3*3+ 1*3+1];
            pOutState[2*3*3+ 1*3+2] = pState[2*3*3+ 2*3+1];
            
            pOutState[2*3*3+ 2*3+0] = pState[2*3*3+ 0*3+0];
            pOutState[2*3*3+ 2*3+1] = pState[2*3*3+ 1*3+0];
            pOutState[2*3*3+ 2*3+2] = pState[2*3*3+ 2*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+0] = pState[0*3*3+ 0*3+0];
            pOutState[4*3*3+ 1*3+0] = pState[0*3*3+ 1*3+0];
            pOutState[4*3*3+ 2*3+0] = pState[0*3*3+ 2*3+0];
            
            pOutState[5*3*3+ 0*3+0] = pState[1*3*3+ 2*3+2];
            pOutState[5*3*3+ 1*3+0] = pState[1*3*3+ 1*3+2];
            pOutState[5*3*3+ 2*3+0] = pState[1*3*3+ 0*3+2];

            pOutState[1*3*3+ 0*3+2] = pState[4*3*3+ 2*3+0];
            pOutState[1*3*3+ 1*3+2] = pState[4*3*3+ 1*3+0];
            pOutState[1*3*3+ 2*3+2] = pState[4*3*3+ 0*3+0];

            pOutState[0*3*3+ 0*3+0] = pState[5*3*3+ 0*3+0];
            pOutState[0*3*3+ 1*3+0] = pState[5*3*3+ 1*3+0];
            pOutState[0*3*3+ 2*3+0] = pState[5*3*3+ 2*3+0];
            break;
         case 8: // L2
            // face colors
            pOutState[2*3*3+ 0*3+0] = pState[2*3*3+ 2*3+2];
            pOutState[2*3*3+ 0*3+1] = pState[2*3*3+ 2*3+1];
            pOutState[2*3*3+ 0*3+2] = pState[2*3*3+ 2*3+0];

            pOutState[2*3*3+ 1*3+0] = pState[2*3*3+ 1*3+2];
            pOutState[2*3*3+ 1*3+1] = pState[2*3*3+ 1*3+1];
            pOutState[2*3*3+ 1*3+2] = pState[2*3*3+ 1*3+0];
            
            pOutState[2*3*3+ 2*3+0] = pState[2*3*3+ 0*3+2];
            pOutState[2*3*3+ 2*3+1] = pState[2*3*3+ 0*3+1];
            pOutState[2*3*3+ 2*3+2] = pState[2*3*3+ 0*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+0] = pState[5*3*3+ 0*3+0];
            pOutState[4*3*3+ 1*3+0] = pState[5*3*3+ 1*3+0];
            pOutState[4*3*3+ 2*3+0] = pState[5*3*3+ 2*3+0];
            
            pOutState[5*3*3+ 0*3+0] = pState[4*3*3+ 0*3+0];
            pOutState[5*3*3+ 1*3+0] = pState[4*3*3+ 1*3+0];
            pOutState[5*3*3+ 2*3+0] = pState[4*3*3+ 2*3+0];

            pOutState[1*3*3+ 0*3+2] = pState[0*3*3+ 2*3+0];
            pOutState[1*3*3+ 1*3+2] = pState[0*3*3+ 1*3+0];
            pOutState[1*3*3+ 2*3+2] = pState[0*3*3+ 0*3+0];

            pOutState[0*3*3+ 0*3+0] = pState[1*3*3+ 2*3+2];
            pOutState[0*3*3+ 1*3+0] = pState[1*3*3+ 1*3+2];
            pOutState[0*3*3+ 2*3+0] = pState[1*3*3+ 0*3+2];
            break;
         case 9: // R
            // face colors
            pOutState[3*3*3+ 0*3+0] = pState[3*3*3+ 2*3+0];
            pOutState[3*3*3+ 0*3+1] = pState[3*3*3+ 1*3+0];
            pOutState[3*3*3+ 0*3+2] = pState[3*3*3+ 0*3+0];

            pOutState[3*3*3+ 1*3+0] = pState[3*3*3+ 2*3+1];
            pOutState[3*3*3+ 1*3+1] = pState[3*3*3+ 1*3+1];
            pOutState[3*3*3+ 1*3+2] = pState[3*3*3+ 0*3+1];
            
            pOutState[3*3*3+ 2*3+0] = pState[3*3*3+ 2*3+2];
            pOutState[3*3*3+ 2*3+1] = pState[3*3*3+ 1*3+2];
            pOutState[3*3*3+ 2*3+2] = pState[3*3*3+ 0*3+2];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+2] = pState[0*3*3+ 0*3+2];
            pOutState[4*3*3+ 1*3+2] = pState[0*3*3+ 1*3+2];
            pOutState[4*3*3+ 2*3+2] = pState[0*3*3+ 2*3+2];
            
            pOutState[5*3*3+ 0*3+2] = pState[1*3*3+ 2*3+0];
            pOutState[5*3*3+ 1*3+2] = pState[1*3*3+ 1*3+0];
            pOutState[5*3*3+ 2*3+2] = pState[1*3*3+ 0*3+0];

            pOutState[0*3*3+ 0*3+2] = pState[5*3*3+ 0*3+2];
            pOutState[0*3*3+ 1*3+2] = pState[5*3*3+ 1*3+2];
            pOutState[0*3*3+ 2*3+2] = pState[5*3*3+ 2*3+2];

            pOutState[1*3*3+ 0*3+0] = pState[4*3*3+ 2*3+2];
            pOutState[1*3*3+ 1*3+0] = pState[4*3*3+ 1*3+2];
            pOutState[1*3*3+ 2*3+0] = pState[4*3*3+ 0*3+2];
            break;
         case 10: // R'
            // face colors
            pOutState[3*3*3+ 0*3+0] = pState[3*3*3+ 0*3+2];
            pOutState[3*3*3+ 0*3+1] = pState[3*3*3+ 1*3+2];
            pOutState[3*3*3+ 0*3+2] = pState[3*3*3+ 2*3+2];

            pOutState[3*3*3+ 1*3+0] = pState[3*3*3+ 0*3+1];
            pOutState[3*3*3+ 1*3+1] = pState[3*3*3+ 1*3+1];
            pOutState[3*3*3+ 1*3+2] = pState[3*3*3+ 2*3+1];
            
            pOutState[3*3*3+ 2*3+0] = pState[3*3*3+ 0*3+0];
            pOutState[3*3*3+ 2*3+1] = pState[3*3*3+ 1*3+0];
            pOutState[3*3*3+ 2*3+2] = pState[3*3*3+ 2*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+2] = pState[1*3*3+ 2*3+0];
            pOutState[4*3*3+ 1*3+2] = pState[1*3*3+ 1*3+0];
            pOutState[4*3*3+ 2*3+2] = pState[1*3*3+ 0*3+0];
            
            pOutState[5*3*3+ 0*3+2] = pState[0*3*3+ 0*3+2];
            pOutState[5*3*3+ 1*3+2] = pState[0*3*3+ 1*3+2];
            pOutState[5*3*3+ 2*3+2] = pState[0*3*3+ 2*3+2];

            pOutState[0*3*3+ 0*3+2] = pState[4*3*3+ 0*3+2];
            pOutState[0*3*3+ 1*3+2] = pState[4*3*3+ 1*3+2];
            pOutState[0*3*3+ 2*3+2] = pState[4*3*3+ 2*3+2];

            pOutState[1*3*3+ 0*3+0] = pState[5*3*3+ 2*3+2];
            pOutState[1*3*3+ 1*3+0] = pState[5*3*3+ 1*3+2];
            pOutState[1*3*3+ 2*3+0] = pState[5*3*3+ 0*3+2];
            break;
         case 11: // R2
            // face colors
            pOutState[3*3*3+ 0*3+0] = pState[3*3*3+ 2*3+2];
            pOutState[3*3*3+ 0*3+1] = pState[3*3*3+ 2*3+1];
            pOutState[3*3*3+ 0*3+2] = pState[3*3*3+ 2*3+0];

            pOutState[3*3*3+ 1*3+0] = pState[3*3*3+ 1*3+2];
            pOutState[3*3*3+ 1*3+1] = pState[3*3*3+ 1*3+1];
            pOutState[3*3*3+ 1*3+2] = pState[3*3*3+ 1*3+0];
            
            pOutState[3*3*3+ 2*3+0] = pState[3*3*3+ 0*3+2];
            pOutState[3*3*3+ 2*3+1] = pState[3*3*3+ 0*3+1];
            pOutState[3*3*3+ 2*3+2] = pState[3*3*3+ 0*3+0];

            // outsize of face colors, up, down, left, right
            pOutState[4*3*3+ 0*3+2] = pState[5*3*3+ 0*3+2];
            pOutState[4*3*3+ 1*3+2] = pState[5*3*3+ 1*3+2];
            pOutState[4*3*3+ 2*3+2] = pState[5*3*3+ 2*3+2];
            
            pOutState[5*3*3+ 0*3+2] = pState[4*3*3+ 0*3+2];
            pOutState[5*3*3+ 1*3+2] = pState[4*3*3+ 1*3+2];
            pOutState[5*3*3+ 2*3+2] = pState[4*3*3+ 2*3+2];

            pOutState[0*3*3+ 0*3+2] = pState[1*3*3+ 2*3+0];
            pOutState[0*3*3+ 1*3+2] = pState[1*3*3+ 1*3+0];
            pOutState[0*3*3+ 2*3+2] = pState[1*3*3+ 0*3+0];

            pOutState[1*3*3+ 0*3+0] = pState[0*3*3+ 2*3+2];
            pOutState[1*3*3+ 1*3+0] = pState[0*3*3+ 1*3+2];
            pOutState[1*3*3+ 2*3+0] = pState[0*3*3+ 0*3+2];
            break;
         case 12: // U
            // face colors
            pOutState[4*3*3+ 0*3+0] = pState[4*3*3+ 2*3+0];
            pOutState[4*3*3+ 0*3+1] = pState[4*3*3+ 1*3+0];
            pOutState[4*3*3+ 0*3+2] = pState[4*3*3+ 0*3+0];

            pOutState[4*3*3+ 1*3+0] = pState[4*3*3+ 2*3+1];
            pOutState[4*3*3+ 1*3+1] = pState[4*3*3+ 1*3+1];
            pOutState[4*3*3+ 1*3+2] = pState[4*3*3+ 0*3+1];
            
            pOutState[4*3*3+ 2*3+0] = pState[4*3*3+ 2*3+2];
            pOutState[4*3*3+ 2*3+1] = pState[4*3*3+ 1*3+2];
            pOutState[4*3*3+ 2*3+2] = pState[4*3*3+ 0*3+2];
            
            // outsize of face colors, up, down, left, right
            pOutState[1*3*3+ 0*3+0] = pState[2*3*3+ 0*3+0];
            pOutState[1*3*3+ 0*3+1] = pState[2*3*3+ 0*3+1];
            pOutState[1*3*3+ 0*3+2] = pState[2*3*3+ 0*3+2];
            
            pOutState[0*3*3+ 0*3+0] = pState[3*3*3+ 0*3+0];
            pOutState[0*3*3+ 0*3+1] = pState[3*3*3+ 0*3+1];
            pOutState[0*3*3+ 0*3+2] = pState[3*3*3+ 0*3+2];

            pOutState[2*3*3+ 0*3+0] = pState[0*3*3+ 0*3+0];
            pOutState[2*3*3+ 0*3+1] = pState[0*3*3+ 0*3+1];
            pOutState[2*3*3+ 0*3+2] = pState[0*3*3+ 0*3+2];

            pOutState[3*3*3+ 0*3+0] = pState[1*3*3+ 0*3+0];
            pOutState[3*3*3+ 0*3+1] = pState[1*3*3+ 0*3+1];
            pOutState[3*3*3+ 0*3+2] = pState[1*3*3+ 0*3+2];
            break;
         case 13: // U'
            // face colors
            pOutState[4*3*3+ 0*3+0] = pState[4*3*3+ 0*3+2];
            pOutState[4*3*3+ 0*3+1] = pState[4*3*3+ 1*3+2];
            pOutState[4*3*3+ 0*3+2] = pState[4*3*3+ 2*3+2];

            pOutState[4*3*3+ 1*3+0] = pState[4*3*3+ 0*3+1];
            pOutState[4*3*3+ 1*3+1] = pState[4*3*3+ 1*3+1];
            pOutState[4*3*3+ 1*3+2] = pState[4*3*3+ 2*3+1];
            
            pOutState[4*3*3+ 2*3+0] = pState[4*3*3+ 0*3+0];
            pOutState[4*3*3+ 2*3+1] = pState[4*3*3+ 1*3+0];
            pOutState[4*3*3+ 2*3+2] = pState[4*3*3+ 2*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[1*3*3+ 0*3+0] = pState[3*3*3+ 0*3+0];
            pOutState[1*3*3+ 0*3+1] = pState[3*3*3+ 0*3+1];
            pOutState[1*3*3+ 0*3+2] = pState[3*3*3+ 0*3+2];
            
            pOutState[0*3*3+ 0*3+0] = pState[2*3*3+ 0*3+0];
            pOutState[0*3*3+ 0*3+1] = pState[2*3*3+ 0*3+1];
            pOutState[0*3*3+ 0*3+2] = pState[2*3*3+ 0*3+2];

            pOutState[2*3*3+ 0*3+0] = pState[1*3*3+ 0*3+0];
            pOutState[2*3*3+ 0*3+1] = pState[1*3*3+ 0*3+1];
            pOutState[2*3*3+ 0*3+2] = pState[1*3*3+ 0*3+2];

            pOutState[3*3*3+ 0*3+0] = pState[0*3*3+ 0*3+0];
            pOutState[3*3*3+ 0*3+1] = pState[0*3*3+ 0*3+1];
            pOutState[3*3*3+ 0*3+2] = pState[0*3*3+ 0*3+2];
            break;
         case 14: // U2
            // face colors
            pOutState[4*3*3+ 0*3+0] = pState[4*3*3+ 2*3+2];
            pOutState[4*3*3+ 0*3+1] = pState[4*3*3+ 2*3+1];
            pOutState[4*3*3+ 0*3+2] = pState[4*3*3+ 2*3+0];

            pOutState[4*3*3+ 1*3+0] = pState[4*3*3+ 1*3+2];
            pOutState[4*3*3+ 1*3+1] = pState[4*3*3+ 1*3+1];
            pOutState[4*3*3+ 1*3+2] = pState[4*3*3+ 1*3+0];
            
            pOutState[4*3*3+ 2*3+0] = pState[4*3*3+ 0*3+2];
            pOutState[4*3*3+ 2*3+1] = pState[4*3*3+ 0*3+1];
            pOutState[4*3*3+ 2*3+2] = pState[4*3*3+ 0*3+0];

            // outsize of face colors, up, down, left, right
            pOutState[1*3*3+ 0*3+0] = pState[0*3*3+ 0*3+0];
            pOutState[1*3*3+ 0*3+1] = pState[0*3*3+ 0*3+1];
            pOutState[1*3*3+ 0*3+2] = pState[0*3*3+ 0*3+2];
            
            pOutState[0*3*3+ 0*3+0] = pState[1*3*3+ 0*3+0];
            pOutState[0*3*3+ 0*3+1] = pState[1*3*3+ 0*3+1];
            pOutState[0*3*3+ 0*3+2] = pState[1*3*3+ 0*3+2];

            pOutState[2*3*3+ 0*3+0] = pState[3*3*3+ 0*3+0];
            pOutState[2*3*3+ 0*3+1] = pState[3*3*3+ 0*3+1];
            pOutState[2*3*3+ 0*3+2] = pState[3*3*3+ 0*3+2];

            pOutState[3*3*3+ 0*3+0] = pState[2*3*3+ 0*3+0];
            pOutState[3*3*3+ 0*3+1] = pState[2*3*3+ 0*3+1];
            pOutState[3*3*3+ 0*3+2] = pState[2*3*3+ 0*3+2];
            break;
         case 15: // D
            // face colors
            pOutState[5*3*3+ 0*3+0] = pState[5*3*3+ 2*3+0];
            pOutState[5*3*3+ 0*3+1] = pState[5*3*3+ 1*3+0];
            pOutState[5*3*3+ 0*3+2] = pState[5*3*3+ 0*3+0];

            pOutState[5*3*3+ 1*3+0] = pState[5*3*3+ 2*3+1];
            pOutState[5*3*3+ 1*3+1] = pState[5*3*3+ 1*3+1];
            pOutState[5*3*3+ 1*3+2] = pState[5*3*3+ 0*3+1];
            
            pOutState[5*3*3+ 2*3+0] = pState[5*3*3+ 2*3+2];
            pOutState[5*3*3+ 2*3+1] = pState[5*3*3+ 1*3+2];
            pOutState[5*3*3+ 2*3+2] = pState[5*3*3+ 0*3+2];
            
            // outsize of face colors, up, down, left, right
            pOutState[0*3*3+ 2*3+0] = pState[2*3*3+ 2*3+0];
            pOutState[0*3*3+ 2*3+1] = pState[2*3*3+ 2*3+1];
            pOutState[0*3*3+ 2*3+2] = pState[2*3*3+ 2*3+2];
            
            pOutState[1*3*3+ 2*3+0] = pState[3*3*3+ 2*3+0];
            pOutState[1*3*3+ 2*3+1] = pState[3*3*3+ 2*3+1];
            pOutState[1*3*3+ 2*3+2] = pState[3*3*3+ 2*3+2];
            
            pOutState[2*3*3+ 2*3+0] = pState[1*3*3+ 2*3+0];
            pOutState[2*3*3+ 2*3+1] = pState[1*3*3+ 2*3+1];
            pOutState[2*3*3+ 2*3+2] = pState[1*3*3+ 2*3+2];

            pOutState[3*3*3+ 2*3+0] = pState[0*3*3+ 2*3+0];
            pOutState[3*3*3+ 2*3+1] = pState[0*3*3+ 2*3+1];
            pOutState[3*3*3+ 2*3+2] = pState[0*3*3+ 2*3+2];
            break;
         case 16: // D'
            // face colors
            pOutState[5*3*3+ 0*3+0] = pState[5*3*3+ 0*3+2];
            pOutState[5*3*3+ 0*3+1] = pState[5*3*3+ 1*3+2];
            pOutState[5*3*3+ 0*3+2] = pState[5*3*3+ 2*3+2];

            pOutState[5*3*3+ 1*3+0] = pState[5*3*3+ 0*3+1];
            pOutState[5*3*3+ 1*3+1] = pState[5*3*3+ 1*3+1];
            pOutState[5*3*3+ 1*3+2] = pState[5*3*3+ 2*3+1];
            
            pOutState[5*3*3+ 2*3+0] = pState[5*3*3+ 0*3+0];
            pOutState[5*3*3+ 2*3+1] = pState[5*3*3+ 1*3+0];
            pOutState[5*3*3+ 2*3+2] = pState[5*3*3+ 2*3+0];
            
            // outsize of face colors, up, down, left, right
            pOutState[0*3*3+ 2*3+0] = pState[3*3*3+ 2*3+0];
            pOutState[0*3*3+ 2*3+1] = pState[3*3*3+ 2*3+1];
            pOutState[0*3*3+ 2*3+2] = pState[3*3*3+ 2*3+2];
            
            pOutState[1*3*3+ 2*3+0] = pState[2*3*3+ 2*3+0];
            pOutState[1*3*3+ 2*3+1] = pState[2*3*3+ 2*3+1];
            pOutState[1*3*3+ 2*3+2] = pState[2*3*3+ 2*3+2];
            
            pOutState[2*3*3+ 2*3+0] = pState[0*3*3+ 2*3+0];
            pOutState[2*3*3+ 2*3+1] = pState[0*3*3+ 2*3+1];
            pOutState[2*3*3+ 2*3+2] = pState[0*3*3+ 2*3+2];

            pOutState[3*3*3+ 2*3+0] = pState[1*3*3+ 2*3+0];
            pOutState[3*3*3+ 2*3+1] = pState[1*3*3+ 2*3+1];
            pOutState[3*3*3+ 2*3+2] = pState[1*3*3+ 2*3+2];
            break;
         case 17: // D2
            // face colors
            pOutState[5*3*3+ 0*3+0] = pState[5*3*3+ 2*3+2];
            pOutState[5*3*3+ 0*3+1] = pState[5*3*3+ 2*3+1];
            pOutState[5*3*3+ 0*3+2] = pState[5*3*3+ 2*3+0];

            pOutState[5*3*3+ 1*3+0] = pState[5*3*3+ 1*3+2];
            pOutState[5*3*3+ 1*3+1] = pState[5*3*3+ 1*3+1];
            pOutState[5*3*3+ 1*3+2] = pState[5*3*3+ 1*3+0];
            
            pOutState[5*3*3+ 2*3+0] = pState[5*3*3+ 0*3+2];
            pOutState[5*3*3+ 2*3+1] = pState[5*3*3+ 0*3+1];
            pOutState[5*3*3+ 2*3+2] = pState[5*3*3+ 0*3+0];

            // outsize of face colors, up, down, left, right            
            pOutState[0*3*3+ 2*3+0] = pState[1*3*3+ 2*3+0];
            pOutState[0*3*3+ 2*3+1] = pState[1*3*3+ 2*3+1];
            pOutState[0*3*3+ 2*3+2] = pState[1*3*3+ 2*3+2];

            pOutState[1*3*3+ 2*3+0] = pState[0*3*3+ 2*3+0];
            pOutState[1*3*3+ 2*3+1] = pState[0*3*3+ 2*3+1];
            pOutState[1*3*3+ 2*3+2] = pState[0*3*3+ 2*3+2];

            pOutState[2*3*3+ 2*3+0] = pState[3*3*3+ 2*3+0];
            pOutState[2*3*3+ 2*3+1] = pState[3*3*3+ 2*3+1];
            pOutState[2*3*3+ 2*3+2] = pState[3*3*3+ 2*3+2];

            pOutState[3*3*3+ 2*3+0] = pState[2*3*3+ 2*3+0];
            pOutState[3*3*3+ 2*3+1] = pState[2*3*3+ 2*3+1];
            pOutState[3*3*3+ 2*3+2] = pState[2*3*3+ 2*3+2];
            break;
      };
   return pOutState;
   }

inline bool isf2l(char *pState)
   {
         //'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', //F 0
         //'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', //B 1
         //'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', //L 2
         //'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o', //R 3
         //'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', 'y', //U 4
         //'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'  //D 5
   
   if(strncmp(&pState[5*3*3], "wwwwwwwww", 9)==0 &&
      strncmp(&pState[0*3*3 + 3], "gggggg", 6)==0 &&
      strncmp(&pState[1*3*3 + 3], "bbbbbb", 6)==0 &&
      strncmp(&pState[2*3*3 + 3], "rrrrrr", 6)==0 &&
      strncmp(&pState[3*3*3 + 3], "oooooo", 6)==0)
      return true;
   return false;
   }         

inline bool isyellowdone(char *pState)
   {
   if(strncmp(&pState[4*3*3], "yyyyyyyyy", 9)==0)
      return true;
   return false;
   }

inline bool isplldone(char *pState) // assumes f2l state
   {
   if(strncmp(&pState[4*3*3], "yyyyyyyyy", 9)==0 &&
      pState[0*3*3 + 0] == pState[0*3*3 + 1] && pState[0*3*3 + 1] == pState[0*3*3 + 2] &&
      pState[1*3*3 + 0] == pState[1*3*3 + 1] && pState[1*3*3 + 1] == pState[1*3*3 + 2] &&
      pState[2*3*3 + 0] == pState[2*3*3 + 1] && pState[2*3*3 + 1] == pState[2*3*3 + 2] &&
      pState[3*3*3 + 0] == pState[3*3*3 + 1] && pState[3*3*3 + 1] == pState[3*3*3 + 2])
      return true;
   return false;
   }
   
void UTheAlg(char *pMoves, int iN) // Do a U on the Algorythm (list of integers mapping to szPossibleMoves).
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 0: pMoves[iA] = 6; break;
         case 1: pMoves[iA] = 7; break;
         case 2: pMoves[iA] = 8; break;
         case 3: pMoves[iA] = 9; break;
         case 4: pMoves[iA] = 10; break;
         case 5: pMoves[iA] = 11; break;
         case 6: pMoves[iA] = 3; break;
         case 7: pMoves[iA] = 4; break;
         case 8: pMoves[iA] = 5; break;
         case 9: pMoves[iA] = 0; break;
         case 10: pMoves[iA] = 1; break;
         case 11: pMoves[iA] = 2; break;
         };
      }
   }
   
void UPrimeTheAlg(char *pMoves, int iN) // Do a U' on the Algorythm (list of integers mapping to szPossibleMoves).
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 0: pMoves[iA] = 9; break;
         case 1: pMoves[iA] = 10; break;
         case 2: pMoves[iA] = 11; break;
         case 3: pMoves[iA] = 6; break;
         case 4: pMoves[iA] = 7; break;
         case 5: pMoves[iA] = 8; break;
         case 6: pMoves[iA] = 0; break;
         case 7: pMoves[iA] = 1; break;
         case 8: pMoves[iA] = 2; break;
         case 9: pMoves[iA] = 3; break;
         case 10: pMoves[iA] = 4; break;
         case 11: pMoves[iA] = 5; break;
         };
      }
   }

void xTheAlg(char *pMoves, int iN)
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 0: pMoves[iA] = 12; break;
         case 1: pMoves[iA] = 13; break;
         case 2: pMoves[iA] = 14; break;
         case 3: pMoves[iA] = 15; break;
         case 4: pMoves[iA] = 16; break;
         case 5: pMoves[iA] = 17; break;
         case 12: pMoves[iA] = 3; break;
         case 13: pMoves[iA] = 4; break;
         case 14: pMoves[iA] = 5; break;
         case 15: pMoves[iA] = 0; break;
         case 16: pMoves[iA] = 1; break;
         case 17: pMoves[iA] = 2; break;
         };
      }
   }

void xPrimeTheAlg(char *pMoves, int iN)
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 0: pMoves[iA] = 15; break;
         case 1: pMoves[iA] = 16; break;
         case 2: pMoves[iA] = 17; break;
         case 3: pMoves[iA] = 12; break;
         case 4: pMoves[iA] = 13; break;
         case 5: pMoves[iA] = 14; break;
         case 12: pMoves[iA] = 0; break;
         case 13: pMoves[iA] = 1; break;
         case 14: pMoves[iA] = 2; break;
         case 15: pMoves[iA] = 3; break;
         case 16: pMoves[iA] = 4; break;
         case 17: pMoves[iA] = 5; break;
         };
      }
   }

void yTheAlg(char *pMoves, int iN)
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 0: pMoves[iA] = 6; break;
         case 1: pMoves[iA] = 7; break;
         case 2: pMoves[iA] = 8; break;
         case 3: pMoves[iA] = 9; break;
         case 4: pMoves[iA] = 10; break;
         case 5: pMoves[iA] = 11; break;
         case 6: pMoves[iA] = 3; break;
         case 7: pMoves[iA] = 4; break;
         case 8: pMoves[iA] = 5; break;
         case 9: pMoves[iA] = 0; break;
         case 10: pMoves[iA] = 1; break;
         case 11: pMoves[iA] = 2; break;
         };
      }
   }

void yPrimeTheAlg(char *pMoves, int iN)
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 0: pMoves[iA] = 9; break;
         case 1: pMoves[iA] = 10; break;
         case 2: pMoves[iA] = 11; break;
         case 3: pMoves[iA] = 6; break;
         case 4: pMoves[iA] = 7; break;
         case 5: pMoves[iA] = 8; break;
         case 6: pMoves[iA] = 0; break;
         case 7: pMoves[iA] = 1; break;
         case 8: pMoves[iA] = 2; break;
         case 9: pMoves[iA] = 3; break;
         case 10: pMoves[iA] = 4; break;
         case 11: pMoves[iA] = 5; break;
         };
      }
   }

void zTheAlg(char *pMoves, int iN)
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 6: pMoves[iA] = 12; break;
         case 7: pMoves[iA] = 13; break;
         case 8: pMoves[iA] = 14; break;
         case 9: pMoves[iA] = 15; break;
         case 10: pMoves[iA] = 16; break;
         case 11: pMoves[iA] = 17; break;
         case 12: pMoves[iA] = 9; break;
         case 13: pMoves[iA] = 10; break;
         case 14: pMoves[iA] = 11; break;
         case 15: pMoves[iA] = 6; break;
         case 16: pMoves[iA] = 7; break;
         case 17: pMoves[iA] = 8; break;
         };
      }
   }

void zPrimeTheAlg(char *pMoves, int iN)
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 6: pMoves[iA] = 15; break;
         case 7: pMoves[iA] = 16; break;
         case 8: pMoves[iA] = 17; break;
         case 9: pMoves[iA] = 12; break;
         case 10: pMoves[iA] = 13; break;
         case 11: pMoves[iA] = 14; break;
         case 12: pMoves[iA] = 6; break;
         case 13: pMoves[iA] = 7; break;
         case 14: pMoves[iA] = 8; break;
         case 15: pMoves[iA] = 9; break;
         case 16: pMoves[iA] = 10; break;
         case 17: pMoves[iA] = 11; break;
         };
      }
   }

void ReverseTheAlg(char *pMoves, int iN) // rewrite the Alg backwards U' becomes U and last move to first move.
   {
   //                         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
   //char szPossibeMoves[] = "F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2";
   for(int iA=0; iA<iN; ++iA)
      {
      switch(pMoves[iA])
         {
         case 0: pMoves[iA] = 1; break;
         case 1: pMoves[iA] = 0; break;
         case 3: pMoves[iA] = 4; break;
         case 4: pMoves[iA] = 3; break;
         case 6: pMoves[iA] = 7; break;
         case 7: pMoves[iA] = 6; break;
         case 9: pMoves[iA] = 10; break;
         case 10: pMoves[iA] = 9; break;
         case 12: pMoves[iA] = 13; break;
         case 13: pMoves[iA] = 12; break;
         case 15: pMoves[iA] = 16; break;
         case 16: pMoves[iA] = 15; break;
         };      
      }
   for(int iA=0; iA<iN && iA<iN-iA-1; ++iA)
      {
      int iWas = pMoves[iA];
      pMoves[iA] = pMoves[iN-iA-1];
      pMoves[iN-iA-1] = iWas;
      }
   }

void printchart(char *pColorChart)
   {
   for(int y1=0; y1<5; ++y1)
      {
      if(y1!=4)
         printf("[%-9.9s]\n", &pColorChart[y1*9]);
      else 
         printf("[%s]\n", &pColorChart[y1*9]);
      }
   }

void printchart(char *pColorChart, FILE *fp)
   {
   for(int y1=0; y1<5; ++y1)
      {
      if(y1!=4)
         fprintf(fp, "[%-9.9s]\n", &pColorChart[y1*9]);
      else 
         fprintf(fp, "[%s]\n", &pColorChart[y1*9]);
      }   
   }

inline bool bMovesCompatable_v2(char a, char b) //fastest
   {
   if(a == b) return false;
   if((std::max(a,b) - std::min(a,b)) >= 3) return true;      
   if(b > a)
      {
      if((a % 3) > (b % 3))
         return true;
      return false;
      }
   if((b % 3) > (a % 3))
      return true;
   return false;
   }

inline bool bMovesCompatable_v3(char a, char b)
   {
   if(a == b) return false;
   if(a > b) 
      { 
      if(a - b >= 3) return true; 
      if((b % 3) > (a % 3))
         return true;
      return false;
      }
   if(b - a >= 3) return true;
   if((a % 3) > (b % 3))
      return true;
   return false;
   }

inline bool bMovesCompatable_v4(char a, char b) //are these adjacent moves ok?
   {
   //if(a == b) return false;
   //if((std::max(a,b) - std::min(a,b)) >= 3) return true;      
   if(a - b >= 3 || b - a >= 3) return true;
   if(b > a)
      {
      if((a % 3) > (b % 3))
         return true;
      return false;
      }
   if(a > b)
      {
      if((b % 3) > (a % 3))
         return true;
      return false;
      }
   return false;
   }
   
inline bool bMovesCompatable_v1(char a, char b) //are these adjacent moves ok?
   {
   if(a == b) return false;
   if((std::max(a,b) - std::min(a,b)) >= 3) return true;
   if((a / 3) == (b / 3))
      return false;
   return true;
   }

struct Primary_T
   {
   int iOLL;
   char aMovesScramble[60];
   char szScramble[60*4];
   char szSolution[60*4];
   int iMoveCount;
   int iMoveCount_WithF2is2Moves;
   int iaPrimaryIndex;
   };

int iPossibleMoves = 0;
std::map<char, char *> mPossibleMoves;
char aMovesStart[60] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int iStartMovesCount = 1;
char aMovesStop[60] =  {0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,3,0,17};
char *aState = NULL;
bool m_bFirst = true;     // I used m_ to mean the value needs to be copied for the multi-threading because the thread changes it at some point.
bool m_bOptionF = false; 
char m_cOptionR = 0;      // must be with the -f option, this option should be the last argument.
                          // Says to try the random number generator to moves within already known algs to come up with more algs.                           
char cOptionR_rnd_value = -1;
char cOptionR_add_value = -1;
bool m_bOptionS = false;
bool bReenableOptionS_at_K = false;
bool bOptionK = false;    // keep going (dont exit), use -start if provided and continue from there, ignores -r.
char szOptionF_File[2048];

typedef struct aMoves_T
   { 
   char aMoves[61]; // first character will be the length of the scramble. Use this for ITC (Inter Thread Communication)
   };
std::deque<aMoves_T> dqNewMoves;
cmutex cMutex(true);

bool bThreadReady;
int iDoNThreads = 1;
int m_iThreadNum;

char iExit = 0;           // each thread exit will add 1 here.
   
int ASCIItoByte(char *pIn, char *pOut) // returns #of moves
   {
   int iRet = 0;
   mstrtok mtok;
   for(char *pPtr = mtok.mstrtok(pIn, " "); pPtr; pPtr=mtok.mstrtok())
      {
      char szMove[3] = {' ', ' ', 0};
      szMove[0] = pPtr[0];
      szMove[1] = pPtr[1]?pPtr[1]:' ';
      for(auto iter : mPossibleMoves)
         if(strncmp(szMove, iter.second, 2) == 0)
            pOut[iRet++] = iter.first;
      }
   return iRet;
   }

void ByteToASCII(char *pIn, int iInCount, char *pOut)
   {
   pOut[0] = 0;
   char *pTmp;
   for(int iA=0; iA<iInCount; ++iA)   
      {
      pTmp = mPossibleMoves.find(pIn[iA])->second;
      pOut[0] = pTmp[0];
      pOut[1] = pTmp[1];
      pOut[2] = 0;
      pOut+=2;
      }
   }

std::vector<unsigned int> m_vPrimes;

bool IsPrime_v1(int n) // slow
   {
   int i, m = 0, flag = 0;
   m = n / 2;
   for(i = 2; i <= m; i++)
      if(n % i == 0)
         return false;
   if(flag==0)
      return true;
   return false;
   }  

bool IsPrime(int n) 
   {
   if(n == 1)  return false;
   int i = 2;
   while(i*i <= n) 
      {
      if(n % i == 0) return false;
            // This means that n has a factor in between 2 and sqrt(n)
            // So it is not a prime number
      i += 1;
      }
   return true;
   }

time_t tAppStart;

#if 0
#if 1
   printf("Done...\n");
   exit(0);
#endif
#endif

#if 1
int main(int argc, char **argv)
   {
   setArgv0();
#ifdef WINDOWS
#else
   fcntl(0, F_SETFD, FD_CLOEXEC);
#endif   
   initrnd();
   _DBG_flush = true;
#endif

   
#if 0 
   //
   // For testing/development random performance related items...
   //
      {
      auto start = std::chrono::high_resolution_clock::now();
      int iCounter = 100000000;
      while(iCounter--)
         {
         int a = rnd4(18);
         int b = rnd4(18);
         //bMovesCompatable_v2(a,b); //<--- v2 seems better
         if(bMovesCompatable_v2(a,b) != bMovesCompatable_v4(a,b))
            printf("Owh No!\n");
         //if(bMovesCompatable_v2(a,b))
            //if(max(a,b) - min(a,b) < 3)
               //printf("a=%d   b=%d\n", a, b);
         }
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> diff = end-start;   
      printf("After 100000000 diff = %.5f, so thats %llu per second\n", 
         diff, (unsigned long long)(((double)1.0/(double)diff.count())*(double)100000000.0));
      printf("Done...\n");
      exit(0);
      }
#endif

   signal(SIGINT,  MySIG);
   signal(SIGTERM, MySIG);
   //signal(SIGQUIT, MySIG);
   //signal(SIGKILL, MySIG);
   //signal(SIGUSR1, MySIG);
   //signal(SIGSTOP, MySIG);
   //signal(SIGTSTP, MySIG);
   tAppStart = time(NULL);   
   initrnd5();

   //
   // Initialize the OLL recognition map
   //
   for(int iA=0; iA<57; ++iA)
      mOLL.insert(std::pair<std::string, int>{(char*)aOLL[iA], iA});

   //
   // Initialize the mPossibleMoves map
   //
   mstrtok mtok;
   for(char *pPtr = mtok.mstrtok(szPossibeMoves, " "); pPtr!=NULL; pPtr=mtok.mstrtok())
      {      
      mPossibleMoves.insert(std::pair<int, char*>{iPossibleMoves, pPtr});
      iPossibleMoves++;
      }

   DBG();
   if(argc == 2 && stricmp(argv[1], "-h")==0)
      {
      printf("Rubiks Cube 3x3x3 One Look Last Layer (1LLL) Algorithm Generator.\n");
      printf("Copyright 2020 by Matthew Pappas, all rights reserved.\n");
      printf("%s.exe [options] \n", szArgv0);
      printf("Options:\n");
      printf("  -f [file.txt]      process input file, scan for 'Scramble:' before proceeding.\n");
      printf("  -r                 combinations are generated and tried randomly.\n");
      printf("  -r3,12             same as -r except we choose random number 0-3 and add 12 for number of moves. \n");
      printf("                     note: F2 counts as 1 move in this application.\n");
      printf("  -m[number of threads]\n");
      printf("                     chose number of threads to create 1 is the default.\n");
      printf("                     note: application creates numbered folders per thread for which to append its output data,\n");
      printf("                           however, only thread 1 will output data, so only 1 folder will be created.\n");
      printf("                           Also use max #of virtual cores - 2 for system stability.\n");
      printf("  -k                 -f by itself, without -r, will simply read input.txt, then recreate the output\n");
      printf("                     files in folder 1. Adding -k instead of -r means to proceed incrementally, not randomly,\n");
      printf("                     thus using -start and -stop or their defaults.\n");
      printf("  -start 0 0 0       for incremental search (not random) start at this algorithm.\n");
      printf("  -stop 15 15 15     for incremental search (not random) stop at this algorithm.\n");
      printf("                     note: application shouldn't need more than 30 digits but I believe 60 are supported.\n");
      printf("                     use this for -start and -stop, numerical mapping to moves:\n");
      printf("                     0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17\n");
      printf("                     F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2\n");
      printf("  -aU -aU' -ax -ax' -ay -ay' -az -az'\n");
      printf("                     Using one of these options says you are researching the output folders and want to\n");
      printf("                     perform a change to an algorithm, example:\n");
      printf("                     %s.exe -aU \"R U F U' F' R'\" \n", szArgv0);
      printf("                     Output: F U L U'L'F' \n");
      printf("                     And yes, more will be added later. \n");
      printf("Examples:\n");
      printf("  %s.exe -stop 17 17 17 17 17 17\n", szArgv0);
      printf("  type 1\\* > input.txt\n");
      printf("  %s.exe -f input.txt -k -stop 17 17 17 17 17 17 1\n", szArgv0);
      printf("  %s.exe -f input.txt -k\n", szArgv0);
      printf("  %s.exe -f input.txt -r\n", szArgv0);
      printf("  %s.exe -m3 -f input.txt -r\n", szArgv0);
      printf("  type 1\\* 2\\* 3\\* > input.txt\n");
      printf("  %s.exe -f input.txt\n", szArgv0);
      printf("  It takes 4-5 minutes on an i3 to generate all 16416 1LLLs with 1 thread,\n");
      printf("  however it takes time and lots of CPUs to shorten the algorithms,\n");
      printf("  merging all the data daily and restarting.\n");
      printf("  Improvements to come, thank you for playing.\n");
      exit(0);
      }
   
   int iTmp;   
   char *aMoves = (char*)malloc(60);
   memset(aMoves, 0, 60);
   int iMaxMoves;
   
   if(argc > 1) // use input arguments to say starting ranges...
      {
      //
      // -a[move] "The Alg"   
      //
      for(int iA=1; iA<argc; ++iA) 
         if(strncmp(argv[iA], "-a", 2)==0)
            {
            char szAlgASCII[60*5*3+1] = {0};
            char szAlgByte[60*5+1];
            int iCount = ASCIItoByte(argv[iA+1], szAlgByte);           
            if(strcmp(argv[iA], "-aU")==0)
               {
               UTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            else if(strcmp(argv[iA], "-aU'")==0)
               {
               UPrimeTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            else if(strcmp(argv[iA], "-ax")==0)
               {
               xTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            else if(strcmp(argv[iA], "-ax'")==0)
               {
               xPrimeTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            else if(strcmp(argv[iA], "-ay")==0)
               {
               yTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            else if(strcmp(argv[iA], "-ay'")==0)
               {
               yPrimeTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            else if(strcmp(argv[iA], "-az")==0)
               {
               zTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            else if(strcmp(argv[iA], "-az'")==0)
               {
               zPrimeTheAlg(szAlgByte, iCount);
               ByteToASCII(szAlgByte, iCount, szAlgASCII);
               }
            printf("%s\n", szAlgASCII);
            exit(0);
            break;
            }
      for(int iA=1; iA<argc; ++iA) 
         if(strcmp(argv[iA], "-k")==0)
            {
            bOptionK = true;
            break;
            }
      for(int iA=1; iA<argc; ++iA) 
         if(strcmp(argv[iA], "-f")==0)
            {
            iTmp=iA;
            m_bOptionF = true;
            break;
            }            
      if(m_bOptionF) // get scrambles from file, -r[random value 0-n provide n][,add value, result of random value + m]
         {         // example -r example -r3,12
         snprintf(szOptionF_File, sizeof(szOptionF_File), "%s", argv[iTmp+1]);         
         for(int iA=1; iA<argc; ++iA) 
            {
            if(strncmp(argv[iA], "-r", 2)==0)
               {
               m_cOptionR = 1;
               if(strlen(argv[iA]) > 2)
                  cOptionR_rnd_value = atoi(&argv[iA][2]);
               if(strchr(argv[iA], ','))
                  cOptionR_add_value = atoi(strchr(argv[iA], ',')+1);
               }
            }
         }
         
      //
      // -start
      //
      iTmp = -1;
      for(int iA=1; iA<argc; ++iA) 
         if(strcmp(argv[iA], "-start")==0)
            { iTmp=iA; break; }
      if(iTmp!=-1)
         {
         iMaxMoves = 0;
         iStartMovesCount = 0;
         for(int iA=iTmp+1; iA<argc; ++iA)
            {            
            if(!isdigit((unsigned char)argv[iA][0])) break;
            aMoves[iA-(iTmp+1)] = atoi(argv[iA]);
            aMovesStart[iA-(iTmp+1)] = atoi(argv[iA]);
            //printf("start: iA=%d, iTmp=%d offset=%d = %d\n", iA, iTmp, iA-(iTmp+1), aMoves[iA-(iTmp+1)]);
            iMaxMoves++;
            iStartMovesCount++;
            }
         if(iMaxMoves < 1) iMaxMoves = 1;
         if(iStartMovesCount < 1) iStartMovesCount = 1;
         }

      //
      // -stop
      //
      iTmp = -1;
      for(int iA=1; iA<argc; ++iA) 
         if(strcmp(argv[iA], "-stop")==0)
            { iTmp=iA; break; }
      if(iTmp!=-1)
         {         
         m_bOptionS = true;
         memset(aMovesStop, 0, 60);
         for(int iA=iTmp+1; iA<argc; ++iA)
            {
            if(!isdigit((unsigned char)argv[iA][0])) break;
            aMovesStop[iA-(iTmp+1)] = atoi(argv[iA]);
            }
         }

      for(int iA=1; iA<argc; ++iA) 
         if(strncmp(argv[iA], "-m", 2)==0)
            {
            iDoNThreads = atoi(&argv[iA][2]);
            if(iDoNThreads < 1) iDoNThreads = 1;
            break;
            }
                        
      //
      // Misc
      //
      if(m_bOptionS && bOptionK)
         {
         bReenableOptionS_at_K = true;
         m_bOptionS = false;
         }
      } 

   aState = (char*)malloc(6*3*3);
   strncpy(aState, 
      "ggggggggg"   //F 0
      "bbbbbbbbb"   //B 1
      "rrrrrrrrr"   //L 2
      "ooooooooo"   //R 3
      "yyyyyyyyy"   //U 4
      "wwwwwwwww"   //D 5
      , 6*3*3);
   
   if(m_cOptionR)
      {
      int iPrimesTo = 10000000;
      printf(".Calculating primes to %d\n", iPrimesTo); fflush(stdout);
      for(int iA=2; iA<=iPrimesTo; ++iA)
         if(IsPrime(iA))
            {
            m_vPrimes.push_back(iA); //aPrimes[iB++] = iA;
            //if((m_vPrimes.size() % 66000) == 0)
               //printf(".%d is prime...\n", iA); fflush(stdout); // I need a few of these...
            }
      printf(".Done Calculating primes to %d. Found %d primes.\n", iPrimesTo, m_vPrimes.size()); fflush(stdout);
      }
         
   m_iThreadNum = 0;
   for(int iN = 0; iN<iDoNThreads; ++iN)
      {
      bThreadReady = false;
      m_iThreadNum++;
      cthread cThread;
      if(cThread.create([](void*) -> unsigned int 
         { 
         int iThreadNum = m_iThreadNum;
         bool bFirst = m_bFirst;
         bool bOptionF = m_bOptionF;
         char cOptionR = m_cOptionR;
         bool bOptionS = m_bOptionS;
         //
         // Prime numbers. (SAVE TO FILE, THIS TAKES WAY TOO LONG!).
         //
         std::vector<unsigned int> vPrimes = m_vPrimes;
         unsigned int *aPrimes = (unsigned int *)&vPrimes[0];
         int iNumPrimes = vPrimes.size();
         unsigned int g_seed1 = tAppStart * GetCurrentThreadId();
         unsigned int g_seed2 = g_seed1 * 827719;
         unsigned int g_seed3 = g_seed2 * 1755563;
         unsigned int g_seed4 = g_seed3 * 2719631;
         char iPool = 0;
         int iNumPrimes1 = vPrimes.size();
         int iNumPrimes2 = vPrimes.size() - 1;
         int iNumPrimes3 = vPrimes.size() - 2;
         int iNumPrimes4 = vPrimes.size() - 3;
         #define localrnd1(n) ((g_seed1=aPrimes[g_seed1%iNumPrimes1]*g_seed1+3709073)%(n+1))
         #define localrnd2(n) ((g_seed2=aPrimes[g_seed2%iNumPrimes2]*g_seed2+4716053)%(n+1))
         #define localrnd3(n) ((g_seed3=aPrimes[g_seed3%iNumPrimes3]*g_seed3+5737393)%(n+1))
         #define localrnd4(n) ((g_seed4=aPrimes[g_seed4%iNumPrimes4]*g_seed4+6770161)%(n+1))
         #define localrnd(n) (((iPool=(iPool+1)%4)==0)?localrnd1(n):(iPool==1)?localrnd2(n):(iPool==2)?localrnd3(n):localrnd4(n))
         bThreadReady = true;
         
         std::map<std::string, Primary_T> mPrimary; // 1LLL color chart is first, second is Primary_T has OLL # and solution.
         std::map<std::string, Primary_T>::iterator aPrimary[16416];
         int iaPrimary_Curr = 0;
         std::map<int, int> mCountPerOLL; // Count how many 1LLL'ers we found per OLL
         int iMaxMoves = 1;
         char *aMoves = NULL;
         aMoves = (char*)malloc(60);
         memset(aMoves, 0, 60);
         int  iRndBuff_ActualSize =  12000+30;    //65536*60*3;
         char *szRndBuff = (char*)malloc(iRndBuff_ActualSize);
         int iRndBuffSize = 0;
         int ir4CountDown = 1;
         int ir4CountDownMAX =        5000; // major design change... lets try it...
         //int ir4CountDownMAX =   8000000; // okay, fast again. live
         //int ir4CountDownMAX =  10000000; // okay, fast again. live
         //int ir4CountDownMAX =  80000000; // original value
         //int ir4CountDownMAX =    400000; // fast at first, but doesnt find all 16416 1LLLs in 1st 9 min.
         //int ir4CountDownMAX =   4000000; // was live, nothing found
         char cFoundAllMax = 0;
         bool bReadingIn = true;
         std::deque<std::string> dqScrambles; // Process the file backwards
         int iCurrNewMoves = 0;
         char cNewMoves = 0;
         aMoves_T pushed_aMoves;
         
         // Omit processing what we already processed. (saves about a seconds)
         typedef struct Node_T
            {
            char aMoves[60];
            int iMaxMoves;
            };
         auto hash = [](const Node_T& n)
            {
            size_t r = 0;
            for(char iA=0; iA<n.iMaxMoves; ++iA)
               r = (r<<5)+(n.aMoves[iA]+1);
            return std::hash<unsigned long long>()(r);
            };
         auto equal = [](const Node_T& l, const Node_T& r)
            {
            if(l.iMaxMoves != r.iMaxMoves) return false;
            return memcmp(l.aMoves, r.aMoves, r.iMaxMoves)?false:true;
            };
         std::unordered_map<Node_T, char, decltype(hash), decltype(equal)> hmOmit(5, hash, equal); //m(8, hash, equal);
   
         char *aState1 = (char*)malloc(6*3*3);
         char *aState2 = (char*)malloc(6*3*3);
         register char *pCurrState;

         char szFile[2048];
         FILE *fdOLL[57]; // ignoring 0th
         if(iThreadNum == 1)
            for(int iA=0; iA<57; ++iA)
               {
               snprintf(szFile, sizeof(szFile), "%d", iThreadNum);
               mkdir(szFile);
               snprintf(szFile, sizeof(szFile), "%d\\%s_oll_%02d.txt", iThreadNum, szArgv0, iA+1);
               fdOLL[iA] = fopen(szFile, "a+b");
               }
         
         FILE *fpOptionF = NULL;
         if(bOptionF) fpOptionF = fopen(szOptionF_File, "rb");
         
         int iBench = -1;
         int iBench2 = 0;
         int iFoundCount = 0;
         auto start = std::chrono::high_resolution_clock::now();
         while(1)
            {
            iBench++;
            if((iBench % 10000000) == 0) 
               { 
               auto end = std::chrono::high_resolution_clock::now();
               std::chrono::duration<double> diff = end-start;
               if(iMaxMoves-2 >= 0)
                  { 
                  DBGS(".%d.%2.2s%2.2s.diff[%.3f].combos per sec[%llu].Thread[%d].Found[%d].Unique1LLLs[%d]",
                     iMaxMoves, mPossibleMoves.find(aMoves[iMaxMoves-2])->second, 
                     mPossibleMoves.find(aMoves[iMaxMoves-1])->second, diff,
                     (unsigned long long)(((double)1.0/(double)diff.count())*(double)10000000.0), 
                     iThreadNum, iFoundCount,
                     mPrimary.size());
                  }
               start = std::chrono::high_resolution_clock::now();
               cMutex.lock();
               if(iCurrNewMoves < dqNewMoves.size()) cNewMoves = 1;
               cMutex.unlock();
               } 
            if(iBench >= 1000000000)
               {
               iBench = 0;
               iBench2++;
               }
            if(bShutdown) 
               {
               if(iThreadNum == 1)
                  for(int iA=0; iA<57; ++iA)
                     fclose(fdOLL[iA]);
               iExit++;
               return 0;
               }

            if(cNewMoves) // share info between threads, so that only 1 thread writes to files.
               {               
               if(cNewMoves == 1)
                  {
                  pushed_aMoves.aMoves[0] = iMaxMoves;
                  memcpy(&pushed_aMoves.aMoves[1], aMoves, iMaxMoves);
                  cNewMoves = 2;
                  }
               cMutex.lock();
               if(iCurrNewMoves < dqNewMoves.size())
                  {
                  iMaxMoves = dqNewMoves[iCurrNewMoves].aMoves[0];
                  memcpy(aMoves, &dqNewMoves[iCurrNewMoves].aMoves[1], iMaxMoves);
                  iCurrNewMoves++;
                  }
               else
                  {
                  iMaxMoves = pushed_aMoves.aMoves[0];
                  memcpy(aMoves, &pushed_aMoves.aMoves[1], iMaxMoves);
                  cNewMoves = 0;
                  }
               cMutex.unlock();
               }
            else
               {
               if(bOptionF)
                  {
                  if(cOptionR == 2) // once all fgets is done, we can start playing with the algs.
                     {  
                     ir4CountDown--;
                     if(ir4CountDown == 0)
                        {
                        ir4CountDown = ir4CountDownMAX;                     
                        if(iaPrimary_Curr)
                           {
                           iRndBuffSize = 0;
                           while(iRndBuffSize < 60)
                              {
                              auto iter1 = &aPrimary[localrnd(iaPrimary_Curr-1)];      
                              // Ensure compatable moves within entire buffer.
                              // forwards and backwards...
                              char cFwd = localrnd(1);
                              char cNew = (*iter1)->second.aMovesScramble[cFwd?0:(*iter1)->second.iMoveCount-1];
                              if(iRndBuffSize)
                                 {
                                 char cPrev = szRndBuff[iRndBuffSize-1];
                                 if(!bMovesCompatable_v2(cPrev, cNew))
                                    {
                                    char cMid = localrnd(iPossibleMoves-1);
                                    for(; !(bMovesCompatable_v2(cPrev, cMid) && bMovesCompatable_v2(cMid, cNew)); )
                                       cMid = localrnd(iPossibleMoves-1);
                                    szRndBuff[iRndBuffSize++] = cMid;
                                    }
                                 }
                              szRndBuff[iRndBuffSize++] = cNew;
                              for(int iA=1; iA<(*iter1)->second.iMoveCount; ++iA)
                                 szRndBuff[iRndBuffSize++] = (*iter1)->second.aMovesScramble[cFwd?iA:(*iter1)->second.iMoveCount-iA-1];
                              }
                           for(int iA=iRndBuffSize-2; iA>=0; --iA)
                              szRndBuff[iRndBuffSize++] = szRndBuff[iA];
                           }                        
                        }
   
                     //
                     // Okay now try a random combination of moves made from the best stuff on earth.
                     //
#if 1
                     if(cOptionR_rnd_value != -1)
                        {
                        if(cOptionR_add_value != -1)
                           iMaxMoves = localrnd(cOptionR_rnd_value)+cOptionR_add_value;
                        else
                           iMaxMoves = localrnd(cOptionR_rnd_value);
                        }
                     else 
                        iMaxMoves = localrnd((cFoundAllMax?cFoundAllMax:30)-6)+6;
                     register char *pJmpTo =  &szRndBuff[localrnd(60)];
                     register char *pRndPos = &szRndBuff[localrnd(60)];
                     register char iRndJmp = localrnd(iMaxMoves-1)+1;
                     register char iMM = iMaxMoves;
                     for(register char iA=0; iA<iMM; iA++)
                        {
                        if(iRndJmp == iA)
                           {
                           pRndPos = pJmpTo;
                           if(!bMovesCompatable_v2(*pRndPos, aMoves[iA-1])) pRndPos++;
                           for(; iA<iMM; iA++)
                              aMoves[iA] = *pRndPos++;
                           break;
                           }
                        aMoves[iA] = *pRndPos++;
                        }
      
#endif
   
#if 0             
                     // 
                     // Life without RndBuffer, too slow.
                     //
                     if(cOptionR_rnd_value != -1)
                        {
                        if(cOptionR_add_value != -1)
                           iMaxMoves = localrnd(cOptionR_rnd_value)+cOptionR_add_value;
                        else
                           iMaxMoves = localrnd(cOptionR_rnd_value);
                        }
                     else 
                        iMaxMoves = localrnd((cFoundAllMax?cFoundAllMax:30)-6)+6;
                                    
                     int iPick;
                     char *pRndPos;
                     int iPos;
                     char cFwd;
                     char iRndJmp;
                     int iMoveCount;
                     
                     iPick = localrnd(iaPrimary_Curr-1);
                     iMoveCount = aPrimary[iPick]->second.iMoveCount;
                     pRndPos = aPrimary[iPick]->second.aMovesScramble;
                     iPos = localrnd(iMoveCount-2);
                     cFwd = localrnd(1)?1:-1;
                     iRndJmp = localrnd(iMaxMoves-1)+1;
                     
                     aMoves[0] = pRndPos[iPos];
                     iPos += cFwd;
                     iRndJmp--;
                     for(char iA=1; iA<iMaxMoves; iA++, iRndJmp--)
                        {
                        if(iRndJmp == 0 || iPos < 0 || iPos >= iMoveCount)
                           {
                           iPick = localrnd(iaPrimary_Curr-1);
                           iMoveCount = aPrimary[iPick]->second.iMoveCount;
                           pRndPos = aPrimary[iPick]->second.aMovesScramble;
                           iPos = localrnd(iMoveCount-2);
                           cFwd = localrnd(1)?1:-1;
                           iRndJmp = localrnd(iMaxMoves-1)+1;
                           if(!bMovesCompatable_v2(pRndPos[iPos], aMoves[iA-1])) iPos++;
                           }
                        aMoves[iA] = pRndPos[iPos];
                        iPos += cFwd;
                        }
      
#endif
   
#if 0
                     // testing...
                     iMaxMoves = 12; // at 5 moves, 1.3 seconds, at 12 moves its 2.5, so rotation is an issue.
                     aMoves[0] = 0;
                     aMoves[1] = 3;
                     aMoves[2] = 0;
                     aMoves[3] = 3;
                     aMoves[4] = 0;
                     aMoves[5] = 3;
                     aMoves[6] = 0;
                     aMoves[7] = 3;
                     aMoves[8] = 0;
                     aMoves[9] = 3;
                     aMoves[10] = 0;
                     aMoves[11] = 3;
#endif                  
   
#if 0 
                     // 
                     // Turn this on to validate aMoves only has compatable adjacent turns
                     //
                     for(int iA=0, iB=1; iA<iMaxMoves-1 && iB<iMaxMoves; ++iA, ++iB)
                        {
                        if(!bMovesCompatable_v2(aMoves[iA], aMoves[iB]))
                           {
                           printf("Owh No!!!\n"); fflush(stdout);
                           for(int iA=0; iA<iMaxMoves+2; ++iA)
                              printf("[%d]\n", aMoves[iA]);
                           printf("iRndJmp=[%d]\n", iRndJmp);
                           fflush(stdout);
                           pause(99999);
                           }
                        }
#endif
                     }
                  else
                     {
                     if(bReadingIn)
                        {
                        char szBuff[65536];                             
                        while(fgets(szBuff, sizeof(szBuff), fpOptionF))
                           {
                           rem1310(szBuff);
                           trim(szBuff); 
                           if(!szBuff[0]) continue;
                           if(strchr(szBuff, '.')) continue;
                           if(strncmp(szBuff, "Scramble: ", 10)!=0) continue;
                           strcat(szBuff, "    "); 
                           dqScrambles.push_back(&szBuff[10]);
                           }
                        fclose(fpOptionF);
                        fpOptionF = NULL;
                        bReadingIn = false;
                        }
                     if(!bReadingIn)
                        {
                        if(dqScrambles.size())
                           {
                           memset(aMoves, 0, 60);
                           iMaxMoves = ASCIItoByte((char*)dqScrambles.back().c_str(), aMoves);
                           if(iMaxMoves > cFoundAllMax) cFoundAllMax = iMaxMoves;
                           dqScrambles.pop_back();
                           }
                        else
                           {
                           if(mPrimary.size() != 16416) cFoundAllMax = 0;
                           iFoundCount = 0;
                           if(bOptionK)
                              {
                              iMaxMoves = iStartMovesCount;
                              memcpy(aMoves, aMovesStart, 60);
                              bOptionF = false;
                              bFirst = true;
                              if(bReenableOptionS_at_K) bOptionS = true;
                              continue;
                              }
                           if(cOptionR == 0)
                              {
                              printf("Done!\n");
                              break;
                              }
                           cOptionR = 2;
                           continue;
                           }
                        }
                     }              
                  bFirst = true;
                  }
                     
               if(!bFirst)
                  {
                  for(int iM=0;;)
                     {
                     aMoves[iM]++;
                     if(aMoves[iM] >= iPossibleMoves)
                        {
                        if(aMoves[iM+1] >= 2)
                           {
                           for(int iM2=iM; iM2>=0; )
                              {
                              aMoves[iM2] = 0;
                              iM2--;
                              if(iM2 < 0) break;
                              aMoves[iM2] = 3;
                              iM2--;
                              }
                           }
                        else
                           {
                           for(int iM2=iM; iM2>=0; )
                              {
                              aMoves[iM2] = 3;
                              iM2--;
                              if(iM2 < 0) break;
                              aMoves[iM2] = 0;
                              iM2--;
                              }  
                           }                  
                        if(iM == iMaxMoves-1) { iMaxMoves++; break; }
                        iM++;
                        continue;
                        }
                     if(iM < iMaxMoves-1 && !bMovesCompatable_v2(aMoves[iM], aMoves[iM+1]))
                        {
                        continue;
                        }
                     break;
                     }
                  }
               else
                  bFirst = false;
            
               if(bOptionS)
                  {
                  for(int iA=60-1; iA>=0; --iA)
                     {
                     if(aMoves[iA] != aMovesStop[iA])
                        {
                        if(aMoves[iA] > aMovesStop[iA])    // memcmp(aMoves, aMovesStop, 60) == 0) // cost: 13 million / sec
                           {
                           if(iThreadNum == 1)
                              for(int iA=0; iA<57; ++iA)
                                 fclose(fdOLL[iA]);
                           iExit++;
                           return 0;
                           }
                        break;
                        }
                     }
                  }
               }
            
            memcpy(aState1, aState, 6*3*3);
            pCurrState = aState1;            

            char iMM = iMaxMoves;
            for(char iA=0;;) // eh, some speed boost coding it like this...
               {
               rotate(aState1, aMoves[iA++], aState2);
               if(iA == iMM) { pCurrState = aState2; break; }
               rotate(aState2, aMoves[iA++], aState1);
               if(iA == iMM) { pCurrState = aState1; break; }
               }

            //
            // V3
            // Pretty good performance writing it like this...
            //
            register char *p = pCurrState + 3;
            if(*p++ == 'g' && *p++ == 'g' && *p++ == 'g' &&
               *p++ == 'g' && *p++ == 'g' && *p++ == 'g')
               {
               p+=3;
               if(*p++ == 'b' && *p++ == 'b' && *p++ == 'b' &&
                  *p++ == 'b' && *p++ == 'b' && *p++ == 'b')
                  {           
                  p+=3;
                  if(*p++ == 'r' && *p++ == 'r' && *p++ == 'r' &&
                     *p++ == 'r' && *p++ == 'r' && *p++ == 'r')
                     {
                     p+=3;
                     if(*p++ == 'o' && *p++ == 'o' && *p++ == 'o' &&
                        *p++ == 'o' && *p++ == 'o' && *p++ == 'o')
                        {
                        if(p[0] != 'y' || p[1] != 'y' || p[2] != 'y' ||
                           p[3] != 'y' || p[4] != 'y' || p[5] != 'y' ||
                           p[6] != 'y' || p[7] != 'y' || p[8] != 'y')
                           {
                           p+=9;
                           if(*p++ == 'w' && *p++ == 'w' && *p++ == 'w' &&
                              *p++ == 'w' && *p++ == 'w' && *p++ == 'w' &&
                              *p++ == 'w' && *p++ == 'w' && *p   == 'w')
                              {
                              p=NULL;
                              }
                           }
                        }
                     }
                  }
               }
               
            ////////////////////////////
            // V2
            // hmmm actually a huge improvement here...
            // if(strncmp(&pCurrState[4*3*3], "yyyyyyyyy", 9)!=0 &&
            //    strncmp(&pCurrState[5*3*3], "wwwwwwwww", 9)==0 &&
            //    strncmp(&pCurrState[0*3*3 + 3], "gggggg", 6)==0 &&
            //    strncmp(&pCurrState[1*3*3 + 3], "bbbbbb", 6)==0 &&
            //    strncmp(&pCurrState[2*3*3 + 3], "rrrrrr", 6)==0 &&
            //    strncmp(&pCurrState[3*3*3 + 3], "oooooo", 6)==0)
            
            ////////////////////////////
            // V1
            // if(isf2l(pCurrState) &&          
            // !isyellowdone(pCurrState))

            //
            // Omit processing what we already processed. (saves about a second)
            //
            if(!p)
               {
               Node_T nt;
               memcpy(nt.aMoves, aMoves, iMaxMoves);
               nt.iMaxMoves = iMaxMoves;
               if(hmOmit.find(nt) != hmOmit.end())
                  p=pCurrState;
               else         
                  hmOmit.insert(std::pair<Node_T, char>{nt, 1});
               }

            if(!p)
               {        
               int iLimit = 0;         
               while(1)
                  {
                  char szYellows[9*5+1] = {0x0};
                  char *pPtr = szYellows;
                  *pPtr++ = ' '; *pPtr++ = ' ';
                  for(int x1 = 2; x1 >= 0; --x1) {
                     *pPtr++ = pCurrState[1*3*3 + 0*3+x1] == 'y'?'y':' ';
                     *pPtr++ = ' '; }
                  *pPtr++ = ' ';
                  for(int y1 = 0; y1 < 3; ++y1)
                     {
                     *pPtr++ = pCurrState[2*3*3 + 0*3+y1] == 'y'?'y':' ';
                     *pPtr++ = ' ';
                     for(int x1 = 0; x1 < 3; ++x1) {
                        *pPtr++ = pCurrState[4*3*3 + y1*3+x1] == 'y'?'y':' ';
                        *pPtr++ = ' '; }
                     *pPtr++ = pCurrState[3*3*3 + 0*3+(2-y1)] == 'y'?'y':' ';
                     }
                  *pPtr++ = ' '; *pPtr++ = ' ';
                  for(int x1 = 0; x1 < 3; ++x1) {
                     *pPtr++ = pCurrState[0*3*3 + 0*3+x1] == 'y'?'y':' ';
                     *pPtr++ = ' '; }
                  *pPtr++ = ' ';
                  *pPtr = 0;
                                                
                  auto iter = mOLL.find(szYellows);
                  if(iter != mOLL.end())
                     {               
                     char sz1LLL[9*5+1] = {0x0};
                     char *pPtr = sz1LLL;
                     *pPtr++ = ' '; *pPtr++ = ' ';
                     for(int x1 = 2; x1 >= 0; --x1) {
                        *pPtr++ = pCurrState[1*3*3 + 0*3+x1];
                        *pPtr++ = ' '; }
                     *pPtr++ = ' ';
                     for(int y1 = 0; y1 < 3; ++y1)
                        {
                        *pPtr++ = pCurrState[2*3*3 + 0*3+y1];
                        *pPtr++ = ' ';
                        for(int x1 = 0; x1 < 3; ++x1) {
                           *pPtr++ = pCurrState[4*3*3 + y1*3+x1];
                           *pPtr++ = ' '; }
                        *pPtr++ = pCurrState[3*3*3 + 0*3+(2-y1)];
                        }
                     *pPtr++ = ' '; *pPtr++ = ' ';
                     for(int x1 = 0; x1 < 3; ++x1) {
                        *pPtr++ = pCurrState[0*3*3 + 0*3+x1];
                        *pPtr++ = ' '; }
                     *pPtr++ = ' ';
                     *pPtr = 0;
                           
                     auto iter_p = mPrimary.find(sz1LLL);
                     if(iter_p == mPrimary.end())
                        {
                        int iMoveCount = iMaxMoves;
                        int iMoveCount_WithF2is2Moves = iMaxMoves;
                        for(char *pI=aMoves; iMoveCount>0; --iMoveCount, ++pI)
                           if(*pI==2 || *pI==5 || *pI==8 || *pI==11 || *pI==14 || *pI==17)
                              iMoveCount_WithF2is2Moves++;
                        iMoveCount = iMaxMoves;
                        char aMovesScramble[60];
                        memcpy(aMovesScramble, aMoves, 60);
                        if(cNewMoves != 2 && iDoNThreads > 1)
                           {
                           cMutex.lock();
                           aMoves_T stNewData;
                           stNewData.aMoves[0] = iMaxMoves;
                           memcpy(&stNewData.aMoves[1], aMoves, iMaxMoves);
                           dqNewMoves.push_back(stNewData);
                           cMutex.unlock();
                           }
                        char szScramble[60*4];
                        szScramble[0] = 0;
                        for(int iA=0; iA<iMaxMoves; ++iA)
                           sprintf(&szScramble[iA*(2+1)], "%2.2s ", mPossibleMoves.find(aMoves[iA])->second);
                        Primary_T st;
                        st.iOLL = iter->second;
                        memcpy(st.aMovesScramble, aMovesScramble, 60);
                        strcpy(st.szScramble, szScramble);
                        st.szSolution[0] = 0;
                        char aMovesTmp[60];
                        memcpy(aMovesTmp, aMoves, iMaxMoves);
                        ReverseTheAlg(aMovesTmp, iMaxMoves);
                        for(int iA=0; iA<iMaxMoves; ++iA)
                           sprintf(&st.szSolution[iA*(2+1)], "%2.2s ", mPossibleMoves.find(aMovesTmp[iA])->second);
                        st.iMoveCount = iMoveCount;
                        st.iMoveCount_WithF2is2Moves = iMoveCount_WithF2is2Moves;
                        st.iaPrimaryIndex = iaPrimary_Curr;
                        aPrimary[iaPrimary_Curr++] = mPrimary.insert(std::pair<std::string, Primary_T>{sz1LLL, st}).first;
                        std::map<int, int>::iterator iter_o = mCountPerOLL.find(st.iOLL);
                        if(iter_o == mCountPerOLL.end())
                           {
                           mCountPerOLL.insert(std::pair<int, int>{st.iOLL, 0});
                           iter_o = mCountPerOLL.find(st.iOLL);
                           }
                        iter_o->second++;                        
                        //
                        // Print some info, into the text files 1/appname_OLL_01.txt etc...
                        // 
                        char szTimeBuff[50];
#if 0 
                        // Doing multithreading now so lets *not* print out to the screen
                        printf("\n%s: tried %dx%d000000000 combos, OLL is #%d: Found %d for this OLL, %d moves (R2 counts twice), %d moves, Found %d 1LLL!\n", 
                           FmtYYYYMMDD_HH24MISS(szTimeBuff),
                           iBench, iBench2, st.iOLL+1, iter_o->second, 
                           iMoveCount_WithF2is2Moves, iMoveCount, mPrimary.size());
                        printf("%d Rotations to get desired OLL orientation\n", iLimit);
                        printf("Scramble: %s\n", szScramble);
                        printf("Solution: %s\n", st.szSolution);
                        printchart(sz1LLL);
#endif
                        if(iThreadNum == 1)
                           {
                           fprintf(fdOLL[st.iOLL], "\n%s: tried %dx%d000000000 combos, OLL is #%d: Found %d for this OLL, %d moves (R2 counts twice), %d moves, Found %d 1LLL!\n", 
                              FmtYYYYMMDD_HH24MISS(szTimeBuff), 
                              iBench, iBench2, st.iOLL, iter_o->second, 
                              iMoveCount_WithF2is2Moves, iMoveCount, mPrimary.size());
                           fprintf(fdOLL[st.iOLL], "%d Rotations to get desired OLL orientation\n", iLimit);
                           fprintf(fdOLL[st.iOLL], "Scramble: %s\n", szScramble);
                           fprintf(fdOLL[st.iOLL], "Solution: %s\n", st.szSolution);
                           printchart(sz1LLL, fdOLL[st.iOLL]);
                           fflush(fdOLL[st.iOLL]);
                           }
                        iFoundCount++;
                        }
                     else
                        {
                        int iMoveCount = iMaxMoves;
                        int iMoveCount_WithF2is2Moves = iMaxMoves;
                        for(char *pI=aMoves; iMoveCount>0; --iMoveCount, ++pI)
                           if(*pI==2 || *pI==5 || *pI==8 || *pI==11 || *pI==14 || *pI==17)
                              iMoveCount_WithF2is2Moves++;
                        iMoveCount = iMaxMoves;
                        if((iter_p->second.iMoveCount > iMoveCount)
                            ||
                           (iMoveCount == iter_p->second.iMoveCount && 
                            iMoveCount_WithF2is2Moves < iter_p->second.iMoveCount_WithF2is2Moves))
                           {
                           int iCountWas = iter_p->second.iMoveCount;
                           char aMovesScramble[60];
                           memcpy(aMovesScramble, aMoves, 60);
                           if(cNewMoves != 2 && iDoNThreads > 1)
                              {
                              cMutex.lock();
                              aMoves_T stNewData;
                              stNewData.aMoves[0] = iMaxMoves;
                              memcpy(&stNewData.aMoves[1], aMoves, iMaxMoves);
                              dqNewMoves.push_back(stNewData);
                              cMutex.unlock();
                              }                  
                           char szScramble[60*4];
                           szScramble[0] = 0;
                           for(int iA=0; iA<iMaxMoves; ++iA)
                              sprintf(&szScramble[iA*(2+1)], "%2.2s ", mPossibleMoves.find(aMoves[iA])->second);
                           Primary_T st;
                           st.iOLL = iter->second;
                           memcpy(st.aMovesScramble, aMovesScramble, 60);
                           strcpy(st.szScramble, szScramble);                     
                           st.szSolution[0] = 0;
                           char aMovesTmp[60];
                           memcpy(aMovesTmp, aMoves, iMaxMoves);
                           ReverseTheAlg(aMovesTmp, iMaxMoves);
                           for(int iA=0; iA<iMaxMoves; ++iA)
                              sprintf(&st.szSolution[iA*(2+1)], "%2.2s ", mPossibleMoves.find(aMovesTmp[iA])->second);
                           st.iMoveCount = iMoveCount;
                           st.iMoveCount_WithF2is2Moves = iMoveCount_WithF2is2Moves;
                           st.iaPrimaryIndex = iter_p->second.iaPrimaryIndex;                                               
                           mPrimary.erase(iter_p);
                           aPrimary[st.iaPrimaryIndex] = mPrimary.insert(std::pair<std::string, Primary_T>{sz1LLL, st}).first;
                           auto iter_o = mCountPerOLL.find(st.iOLL);
                           char szTimeBuff[50];
#if 0
                           printf("\n%s: tried %dx%d000000000 combos, OLL is #%d: Found %d for this OLL, %d moves (R2 counts twice), %d moves, Found %d 1LLL! (found shorter alg <%d)\n",
                              FmtYYYYMMDD_HH24MISS(szTimeBuff), 
                              iBench, iBench2, st.iOLL+1, iter_o->second, 
                              iMoveCount_WithF2is2Moves, iMoveCount, mPrimary.size(),
                              iCountWas);
                           printf("%d Rotations to get desired OLL orientation\n", iLimit);
                           printf("Scramble: %s\n", szScramble);
                           printf("Solution: %s\n", st.szSolution);
                           printchart(sz1LLL);
#endif                        
                           if(iThreadNum == 1)
                              {
                              fprintf(fdOLL[st.iOLL], "\n%s: tried %dx%d000000000 combos, OLL is #%d: Found %d for this OLL, %d moves (R2 counts twice), %d moves, Found %d 1LLL! (found shorter alg <%d)\n", 
                                 FmtYYYYMMDD_HH24MISS(szTimeBuff), 
                                 iBench, iBench2, st.iOLL+1, iter_o->second, 
                                 iMoveCount_WithF2is2Moves, iMoveCount, mPrimary.size(),
                                 iCountWas);
                              fprintf(fdOLL[st.iOLL], "%d Rotations to get desired OLL orientation\n", iLimit);
                              fprintf(fdOLL[st.iOLL], "Scramble: %s\n", szScramble);
                              fprintf(fdOLL[st.iOLL], "Solution: %s\n", st.szSolution);
                              printchart(sz1LLL, fdOLL[st.iOLL]);
                              fflush(fdOLL[st.iOLL]);
                              }
                           iFoundCount++;
                           }
                        }
                     fflush(stdout);
                     break;
                     }
                  //                
                  // OLL Not Found in the desired orientation, UTheAlg!
                  //
                  iLimit++;
                  if(iLimit == 4)
                     {
                     printf("Owh no! ");
                     for(int iA=0; iA<iMaxMoves; ++iA)
                        printf("%2.2s ", mPossibleMoves.find(aMoves[iA])->second);
                     printf("\n");
                     if(iThreadNum == 1)
                        for(int iA=0; iA<57; ++iA)
                           fclose(fdOLL[iA]);
                     iExit++;
                     return 0;
                     }
                  pCurrState = rotate(pCurrState, 12, pCurrState==aState1?aState2:aState1);
                  char *pPtr2;
                  for(int y1=0; y1<3; ++y1) // change the colors to
                     for(int x1=0; x1<3; ++x1)
                        {
                        pPtr2 = pCurrState + 4*3*3 + y1*3+x1;
                        switch(*pPtr2)
                           {
                           case 'g': *pPtr2 = 'r'; break;
                           case 'r': *pPtr2 = 'b'; break;
                           case 'b': *pPtr2 = 'o'; break;
                           case 'o': *pPtr2 = 'g'; break;
                           };
                        }
                  for(int face=0; face<=3; ++face)
                     for(int x1=0; x1<3; ++x1)
                        {
                        pPtr2 = pCurrState + face*3*3 + 0*3+x1;
                        switch(*pPtr2)
                           {
                           case 'g': *pPtr2 = 'r'; break;
                           case 'r': *pPtr2 = 'b'; break;
                           case 'b': *pPtr2 = 'o'; break;
                           case 'o': *pPtr2 = 'g'; break;
                           };
                        }
                  UTheAlg(aMoves, iMaxMoves);
                  }
               //
               // Get back to original state of aMoves, 
               // ah, but only if we are in combo increment mode.
               //
               if(cOptionR != 2)
                  {
                  if(iLimit == 1)      { UPrimeTheAlg(aMoves, iMaxMoves); }
                  else if(iLimit == 2) { UTheAlg(aMoves, iMaxMoves); UTheAlg(aMoves, iMaxMoves); }
                  else if(iLimit == 3) { UTheAlg(aMoves, iMaxMoves); }
                  }
               }
            };
         if(iThreadNum == 1)
            for(int iA=0; iA<57; ++iA)
               fclose(fdOLL[iA]);
         printf("Main part is done.\n");
         if(bOptionF && iThreadNum == 1)
            {      
            printf("Okay, outputting final results... recreating all text files....\n");
            char szFile[2048];
            for(int iA=0; iA<57; ++iA)
               {
               snprintf(szFile, sizeof(szFile), "%d", iThreadNum);
               mkdir(szFile);
               snprintf(szFile, sizeof(szFile), "%d\\%s_oll_%02d.txt", iThreadNum, szArgv0, iA+1);
               fdOLL[iA] = fopen(szFile, "wb");
               }
            //
            // We want to sort each file from fewest moves to most moves...
            //
            std::multimap<long, std::string> mPriSort;
            for(auto iter : mPrimary)
               mPriSort.insert(std::pair<long, std::string>{
                  iter.second.iOLL       * 1000000 +
                  iter.second.iMoveCount *    1000 +
                  iter.second.iMoveCount_WithF2is2Moves,
                  iter.first
                  });
            for(auto iter2 : mPriSort)
               {    
               auto iter = mPrimary.find(iter2.second.c_str());
               auto iter_o = mCountPerOLL.find(iter->second.iOLL);
               char szTimeBuff[50];
               fprintf(fdOLL[iter->second.iOLL], "\n%s: tried %dx%d000000000 combos, OLL is #%d: Found %d for this OLL, %d moves (R2 counts twice), %d moves, Found %d 1LLL!\n", 
                  FmtYYYYMMDD_HH24MISS(szTimeBuff), 
                  iBench, iBench2, iter->second.iOLL+1, iter_o->second, 
                  iter->second.iMoveCount_WithF2is2Moves, iter->second.iMoveCount, mPrimary.size());
               fprintf(fdOLL[iter->second.iOLL], "Scramble: %s\n", iter->second.szScramble);
               fprintf(fdOLL[iter->second.iOLL], "Solution: %s\n", iter->second.szSolution);
               printchart((char*)iter->first.c_str(), fdOLL[iter->second.iOLL]);
               fflush(fdOLL[iter->second.iOLL]);
               }
            for(int iA=0; iA<57; ++iA)
               fclose(fdOLL[iA]);      
            }
         iExit++;
         return 0; 
         }, 8000000))
         {
         DBGS("Could not start thread, errno=%ld, strerror(errno)=[%s]", errno, strerror(errno));
         iExit++;
         }
      //
      // SetThreadPriority(cThread.hThread, THREAD_PRIORITY_BELOW_NORMAL); //THREAD_PRIORITY_BELOW_NORMAL //THREAD_PRIORITY_NORMAL //THREAD_PRIORITY_ABOVE_NORMAL
      // I so no improvement tinkering with this setting.
      //
      while(bThreadReady == false) { Sleep(1000); }
      printf(".Thread %d is ready and running.\n", m_iThreadNum); fflush(stdout);
      Sleep(4000);
      }  
   printf(".All Threads are running.\n"); fflush(stdout);
   while(iExit != iDoNThreads) { Sleep(1000); }
   printf("Done...\n");
   
   exit(0);

#if 1
    return 0;
}
#endif






