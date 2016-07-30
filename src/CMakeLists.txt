CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT("Sage Aasvogel" NONE)

# klog options
OPTION(KLOG_OUTPUT_TO_HOST_FILE "Output kernel logs to serial.out" ON)

# Extra debug output
OPTION(MMAP_DEBUG "Print physical memory map on startup" OFF)
OPTION(SCHED_DEBUG "Store debug info about threads and processes" OFF)
OPTION(KMEM_EARLY_DEBUG "Track usage of kmalloc_early" OFF)

# Kernel GDB stub
OPTION(KERNEL_GDB_STUB "Include a GDB stub with the kernel" OFF)
OPTION(KERNEL_GDB_WAIT "On boot, immediately break and wait for GDB" OFF)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

SET(SAGE_AASVOGEL_IMG_DIRECTORY ${CMAKE_BINARY_DIR}/img)
SET(SAGE_AASVOGEL_DOC_DIRECTORY ${CMAKE_BINARY_DIR}/doc)
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${SAGE_AASVOGEL_IMG_DIRECTORY})

SET(KERNEL_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/kernel/include)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__SOURCE_FILE__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

SET(QEMU_FLAGS -m 64 -s -cpu Westmere,-de,-syscall,-lm,-vme,enforce -smp 2 -drive file=sa.img,if=floppy,format=raw)

SET(NEXT_SERIAL_PORT 0)

IF(KLOG_OUTPUT_TO_HOST_FILE)
    SET(QEMU_FLAGS ${QEMU_FLAGS} -serial file:serial.out)
    
    SET(KLOG_SERIAL_PORT ${NEXT_SERIAL_PORT})
    MATH(EXPR NEXT_SERIAL_PORT ${NEXT_SERIAL_PORT}+1)
ENDIF()

IF(KERNEL_GDB_STUB)
    SET(QEMU_FLAGS ${QEMU_FLAGS} -chardev pty,id=gdbs -serial chardev:gdbs)
    
    SET(KERNEL_GDB_STUB_SERIAL_PORT ${NEXT_SERIAL_PORT})
    MATH(EXPR NEXT_SERIAL_PORT ${NEXT_SERIAL_PORT}+1)
ENDIF()

ADD_SUBDIRECTORY(acpica)
ADD_SUBDIRECTORY(kernel)

SET(IMG_TEMPLATE ${CMAKE_SOURCE_DIR}/../template.img)
SET(IMG_MNT_DIR /mnt/sa-floppy)

ADD_CUSTOM_TARGET(doc
    DEPENDS kernel-doc
)

ADD_CUSTOM_TARGET(image ALL
    DEPENDS kernel-image
)

ADD_CUSTOM_TARGET(pack-image
    COMMAND cp -f --preserve=ownership ${IMG_TEMPLATE} sa.img
    COMMAND mkdir ${IMG_MNT_DIR}
    COMMAND mount -o loop sa.img ${IMG_MNT_DIR}
    COMMAND cp -r ${SAGE_AASVOGEL_IMG_DIRECTORY}/* ${IMG_MNT_DIR}
    COMMAND sleep 1
    COMMAND umount ${IMG_MNT_DIR}
    COMMAND rm -rf ${IMG_MNT_DIR}
    DEPENDS image)

ADD_CUSTOM_TARGET(emulate
    COMMAND ${QEMU_COMMAND} ${QEMU_FLAGS}
)