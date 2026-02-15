cmd_/home/oem/kernel_process_07/modules.order := {   echo /home/oem/kernel_process_07/unified_vma_inspector.ko; :; } | awk '!x[$$0]++' - > /home/oem/kernel_process_07/modules.order
