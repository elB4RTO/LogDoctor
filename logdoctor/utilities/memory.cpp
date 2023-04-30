
#include "memory.h"

#include "modules/exceptions.h"

// 1: linux, 2:windows, 3:mac
#if defined( Q_OS_DARWIN )
    // Darwin-based systems: macOS, iOS, watchOS and tvOS.
    #include <unistd.h>
    #include <mach/mach.h>
#elif defined( Q_OS_WIN )
    // Microsoft Windows systems
    #include <windows.h>
#elif defined( Q_OS_UNIX )
    // Unix-like systems: Linux, BSD and SysV
    #include <unistd.h>
#else
    #error "System not supported"
#endif


namespace MemOps {

const size_t availableMemory() {
#if defined( Q_OS_DARWIN )
    // Darwin-based systems: macOS, iOS, watchOS and tvOS.
    mach_msg_type_number_t count{ HOST_VM_INFO_COUNT };
    vm_statistics_data_t vmstat;
    if ( host_statistics( mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count ) != KERN_SUCCESS ) {
        throw GenericException("Failed to get host infos", true);
    }
    double n_pages{ vmstat.free_count };
    long page_size{ sysconf( _SC_PAGE_SIZE ) };
    if ( n_pages < 0.0 || page_size < 0l ) {
        return 0ul;
    }
    return static_cast<size_t>( n_pages ) * static_cast<size_t>( page_size );
#elif defined( Q_OS_WIN )
    // Microsoft Windows systems
    MEMORYSTATUSEX mem_statexs;
    mem_statexs.dwLength = sizeof( mem_statexs );
    GlobalMemoryStatusEx( &mem_statexs );
    return static_cast<size_t>( mem_statexs.ullAvailPhys );
#elif defined( Q_OS_UNIX )
    // Unix-like systems: Linux, BSD and SysV
    long n_pages{ sysconf( _SC_AVPHYS_PAGES ) };
    long page_size{ sysconf( _SC_PAGE_SIZE ) };
    if ( n_pages < 0l || page_size < 0l ) {
        return 0ul;
    }
    return static_cast<size_t>( n_pages * page_size );
#else
    #error "System not supported"
#endif
} // availableMemory()

} // namespace MemOps
