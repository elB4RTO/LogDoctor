
#include "memory.h"

#include "modules/exceptions.h"

#if defined( Q_OS_UNIX )
#   include <unistd.h>
#   if defined( Q_OS_DARWIN )
#       include <mach/mach.h>
#   elif defined( Q_OS_BSD4 )
#       include <sys/types.h>
#       include <sys/sysctl.h>
#       include <sys/vmmeter.h>
#   endif
#elif defined( Q_OS_WIN )
#   include <windows.h>
#else
#   error "System not supported"
#endif


namespace MemOps {

size_t availableMemory() {
#if defined( Q_OS_WIN )
    MEMORYSTATUSEX mem_statexs;
    mem_statexs.dwLength = sizeof( mem_statexs );
    GlobalMemoryStatusEx( &mem_statexs );
    return static_cast<size_t>( mem_statexs.ullAvailPhys );
#elif defined( Q_OS_DARWIN )
    mach_msg_type_number_t count{ HOST_VM_INFO_COUNT };
    vm_statistics_data_t vmstat;
    if ( host_statistics( mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count ) != KERN_SUCCESS ) {
        throw GenericException("Failed to get host infos", true);
    }
    const natural_t n_pages{ vmstat.free_count };
    const long page_size{ sysconf( _SC_PAGE_SIZE ) };
    if ( n_pages < 0u || page_size < 0l ) {
        return 0ul;
    }
    return static_cast<size_t>( n_pages ) * static_cast<size_t>( page_size );
#elif defined( Q_OS_BSD4 )
    vmtotal vmt;
    u_int page_size;
    size_t vmt_size{ sizeof(vmt) };
    size_t uint_size{ sizeof(page_size) };
    if ( sysctlbyname("vm.vmtotal", &vmt, &vmt_size, NULL, 0) < 0 ) {
        throw GenericException("Failed to get vmtotal", true);
    }
    if ( sysctlbyname("vm.stats.vm.v_page_size", &page_size, &uint_size, NULL, 0) < 0 ) {
        throw GenericException("Failed to get v_page_size", true);
    }
    return vmt.t_free * static_cast<size_t>( page_size );
#elif defined( Q_OS_UNIX )
    const long n_pages{ sysconf( _SC_AVPHYS_PAGES ) };
    const long page_size{ sysconf( _SC_PAGE_SIZE ) };
    if ( n_pages < 0l || page_size < 0l ) {
        return 0ul;
    }
    return static_cast<size_t>( n_pages * page_size );
#else
#   error "System not supported"
#endif
} // availableMemory()

} // namespace MemOps
