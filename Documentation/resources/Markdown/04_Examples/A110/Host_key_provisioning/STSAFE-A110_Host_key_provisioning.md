# STSAFE-A110_Host_key_provisioning{#STSAFE-A110_Host_key_provisioning}

This example illustrates how to performs host key provisioning of a target STSAFE-A110.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A110_Host_key_provisioning.png} "STSAFE-A110_Host_key_provisioning Example flowchart" width=5cm
	:MAIN;
	:Initialize apps terminal]
	:Initialize STSE device (addr 0x20)]
	:print Host MAC and Host Cipher keys]
	:print WARNING about the lock of target slot]
	:ret = stse_host_key_provisioning|
	if(ret != STSE_OK) then (No)
    else (Yes)
        :print ERROR]
        -[hidden]->
        detach
    endif
	:print "stse_host_key_provisioning : PASS"]
	while (while(1))
	end while
	-[hidden]->
	detach
@enduml

API and services used in the example :
 
- \ref stse_init
- \ref stse_host_key_provisioning

When executed following logs can be seen on the host attached computer terminal window \n

```
 ----------------------------------------------------------------------------------------------------------------
 -                                  STSAFE-A110 Host key provisioning example                                   -
 ----------------------------------------------------------------------------------------------------------------
 -
 - description :
 - This examples illustrates host keys provisioning of a target STSAFE-A110 device
 -
 ----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A110

 - Host mac key to write :

  0x00 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88 0x99 0xAA 0xBB 0xCC 0xDD 0xEE 0xFF

 - Host cipher key to write :

  0x01 0x23 0x45 0x67 0x89 0xAB 0xCD 0xEF 0x01 0x23 0x45 0x67 0x89 0xAB 0xCD 0xEF

 +--------------------------------------------------------------------------------------------------------------+
 |                                                                                                              |
 |                                           /!\ W A R N I N G /!\                                              |
 |                                                                                                              |
 | The program will write test keys in the target STSAFE-A110 and lock the slot                                 |
 |                                                                                                              |
 | Press any key to continue and write the host keys :                                                          |
 |                                                                                                              |
 +--------------------------------------------------------------------------------------------------------------+

 - stse_host_key_provisioning : PASS

```

Applicative scenario : 

- Host key provisioning plaintext