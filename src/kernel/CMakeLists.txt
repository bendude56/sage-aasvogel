CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT("Sage Aasvogel Kernel" C ASM)

SET(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

SET(BOOT_CMDLINE "klog_console_level=8")

IF(KLOG_OUTPUT_TO_HOST_FILE)
    SET(BOOT_CMDLINE "${BOOT_CMDLINE} klog_serial_port=${KLOG_SERIAL_PORT} klog_serial_level=8")
ENDIF()

IF(MMAP_DEBUG)
    ADD_DEFINITIONS(-DMMAP_DEBUG)
ENDIF()

IF(SCHED_DEBUG)
    ADD_DEFINITIONS(-DSCHED_DEBUG)
ENDIF()

IF(KMEM_EARLY_DEBUG)
    ADD_DEFINITIONS(-DKMEM_EARLY_TRACK)
ENDIF()

IF(KERNEL_GDB_STUB)
    ADD_DEFINITIONS(-DGDB_STUB_ENABLED)
    SET(BOOT_CMDLINE "${BOOT_CMDLINE} kernel_gdb_serial=${KERNEL_GDB_STUB_SERIAL_PORT}")
ENDIF()

IF(KERNEL_GDB_WAIT)
    SET(BOOT_CMDLINE "${BOOT_CMDLINE} kernel_gdb_break")
ENDIF()

INCLUDE_DIRECTORIES(${KERNEL_INCLUDE_DIRECTORY})
FILE(GLOB_RECURSE C_SRCS *.c)
FILE(GLOB_RECURSE ASM_SRCS *.S)

ADD_EXECUTABLE(kernel EXCLUDE_FROM_ALL ${C_SRCS} ${ASM_SRCS})
TARGET_LINK_LIBRARIES(kernel acpica)
SET_TARGET_PROPERTIES(kernel PROPERTIES SUFFIX .bin)

ADD_CUSTOM_TARGET(kernel-image
    DEPENDS ${SAGE_AASVOGEL_IMG_DIRECTORY}/kernel.bin
            ${SAGE_AASVOGEL_IMG_DIRECTORY}/boot/menu.cfg
)

ADD_CUSTOM_COMMAND(OUTPUT ${SAGE_AASVOGEL_IMG_DIRECTORY}/kernel.bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kernel> ${SAGE_AASVOGEL_IMG_DIRECTORY}/kernel.bin
    DEPENDS kernel
)

ADD_CUSTOM_COMMAND(OUTPUT ${SAGE_AASVOGEL_IMG_DIRECTORY}/boot/menu.cfg
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SAGE_AASVOGEL_IMG_DIRECTORY}/boot
    COMMAND sed -e \"s/KERNEL_CMDLINE/${BOOT_CMDLINE}/\" ${CMAKE_CURRENT_SOURCE_DIR}/menu.cfg > ${SAGE_AASVOGEL_IMG_DIRECTORY}/boot/menu.cfg
)

ADD_CUSTOM_TARGET(kernel-doc
    COMMAND mkdir -p ${SAGE_AASVOGEL_DOC_DIRECTORY}/kernel
    COMMAND doxygen doxygen-generated.cfg
    DEPENDS doxygen-generated.cfg
)

ADD_CUSTOM_COMMAND(OUTPUT doxygen-generated.cfg
    COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.cfg doxygen-generated.cfg
    COMMAND echo "INCLUDE_PATH = ${KERNEL_INCLUDE_DIRECTORY}" >> doxygen-generated.cfg
    COMMAND echo "OUTPUT_DIRECTORY = ${SAGE_AASVOGEL_DOC_DIRECTORY}/kernel" >> doxygen-generated.cfg
    COMMAND echo "STRIP_FROM_PATH = ${CMAKE_CURRENT_SOURCE_DIR} ${KERNEL_INCLUDE_DIRECTORY}" >> doxygen-generated.cfg
    COMMAND echo "STRIP_FROM_INC_PATH = ${KERNEL_INCLUDE_DIRECTORY}" >> doxygen-generated.cfg
    COMMAND echo "EXCLUDE_PATTERNS = $(realpath ${KERNEL_INCLUDE_DIRECTORY}/acpica)" >> doxygen-generated.cfg
    COMMAND echo "INPUT = ${CMAKE_CURRENT_SOURCE_DIR}" >> doxygen-generated.cfg
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.cfg
    VERBATIM
)
