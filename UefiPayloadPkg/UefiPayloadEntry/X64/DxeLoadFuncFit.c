/** @file
  x64-specifc functionality for DxeLoad.

  Copyright (c) 2024, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/FdtLib.h>
#include <Library/PcdLib.h>
#include "X64/VirtualMemory.h"
#include "UefiPayloadEntry.h"
#define STACK_SIZE  0x20000

extern VOID  *mHobList;

/**
   Transfers control to DxeCore.

   This function performs a CPU architecture specific operations to execute
   the entry point of DxeCore with the parameters of HobList.
   It also installs EFI_END_OF_PEI_PPI to signal the end of PEI phase.

   @param DxeCoreEntryPoint         The entry point of DxeCore.
   @param HobList                   The start of HobList passed to DxeCore.

**/
VOID
HandOffToDxeCore (
  IN EFI_PHYSICAL_ADDRESS  DxeCoreEntryPoint,
  IN EFI_PEI_HOB_POINTERS  HobList
  )
{
  VOID   *BaseOfStack;
  VOID   *TopOfStack;
  UINTN  PageTables;
  VOID   *GhcbBase;
  UINTN  GhcbSize;

  // Initialize floating point operating environment to be compliant with UEFI spec.
  InitializeFloatingPointUnits ();

  //
  // Mask off all legacy 8259 interrupt sources
  //
  IoWrite8 (LEGACY_8259_MASK_REGISTER_MASTER, 0xFF);
  IoWrite8 (LEGACY_8259_MASK_REGISTER_SLAVE, 0xFF);

  //
  // Allocate 128KB for the Stack
  //
  BaseOfStack = AllocatePages (EFI_SIZE_TO_PAGES (STACK_SIZE));
  ASSERT (BaseOfStack != NULL);

  //
  // Compute the top of the stack we were allocated. Pre-allocate a UINTN
  // for safety.
  //
  TopOfStack = (VOID *)((UINTN)BaseOfStack + EFI_SIZE_TO_PAGES (STACK_SIZE) * EFI_PAGE_SIZE - CPU_STACK_ALIGNMENT);
  TopOfStack = ALIGN_POINTER (TopOfStack, CPU_STACK_ALIGNMENT);

  //
  // Get the address and size of the GHCB pages
  //
  GhcbBase = 0;
  GhcbSize = 0;

  PageTables = 0;
  if (FeaturePcdGet (PcdDxeIplBuildPageTables)) {
    //
    // Create page table and save PageMapLevel4 to CR3
    //
    PageTables = CreateIdentityMappingPageTables (
                   (EFI_PHYSICAL_ADDRESS)(UINTN)BaseOfStack,
                   STACK_SIZE,
                   (EFI_PHYSICAL_ADDRESS)(UINTN)GhcbBase,
                   GhcbSize
                   );
  } else {
    //
    // Set NX for stack feature also require PcdDxeIplBuildPageTables be TRUE
    // for the DxeIpl and the DxeCore are both X64.
    //
    ASSERT (PcdGetBool (PcdSetNxForStack) == FALSE);
    ASSERT (PcdGetBool (PcdCpuStackGuard) == FALSE);
  }

  if (FeaturePcdGet (PcdDxeIplBuildPageTables)) {
    AsmWriteCr3 (PageTables);
  }

  //
  // Update the contents of BSP stack HOB to reflect the real stack info passed to DxeCore.
  //
  UpdateStackHob ((EFI_PHYSICAL_ADDRESS)(UINTN)BaseOfStack, STACK_SIZE);

  //
  // Transfer the control to the entry point of DxeCore.
  //
  SwitchStack (
    (SWITCH_STACK_ENTRY_POINT)(UINTN)DxeCoreEntryPoint,
    HobList.Raw,
    NULL,
    TopOfStack
    );
}

/**
  Entry point to the C language phase of UEFI payload.
  @param[in]   BootloaderParameter    The starting address of bootloader parameter block.
  @retval      It will not return if SUCCESS, and return error when passing bootloader parameter.
**/
EFI_STATUS
EFIAPI
_ModuleEntryPoint (
  IN UINTN  BootloaderParameter
  )
{
  return FitUplEntryPoint (BootloaderParameter);
}
