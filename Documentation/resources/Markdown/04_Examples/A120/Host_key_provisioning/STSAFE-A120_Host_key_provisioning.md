# STSAFE-A120_Host_key_provisioning{#STSAFE-A120_Host_key_provisioning}

This example illustrates how to performs host key provisioning of a target STSAFE-A120.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Host_key_provisioning.png} "STSAFE-A120_Host_key_provisioning Example flowchart" width=5cm
	:MAIN;
	:Initialize apps terminal]
	:Initialize STSE device (addr 0x20)]
	:print Host MAC and Host Cipher keys]
	:ret = stsafea_query_host_key_provisioning_ctrl_fields|
	if(ret != STSE_OK) then (No)
    else (Yes)
        :print ERROR]
        -[hidden]->
        detach
    endif
	:print Host key provisioning control fields]
	:print WARNING about the lock of target slot]
	if(provisioning_ctrl_fields.change_right == 0) then (No)
		:stsafea_put_host_key_provisioning_ctrl_fields|
		if(ret != STSE_OK) then (No)
		else (Yes)
			:print ERROR]
			-[hidden]->
			detach
		endif
    else (Yes)
		if(provisioning_ctrl_fields.reprovision == 0) then (No)
			:print "control fields already set"]
		else (Yes)
			:print ERROR]
			-[hidden]->
			detach
		endif
    endif
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
- \ref stsafea_query_host_key_provisioning_ctrl_fields
- \ref stsafea_put_host_key_provisioning_ctrl_fields

When executed following logs can be seen on the host attached computer terminal window \n

```
 ----------------------------------------------------------------------------------------------------------------
 -                                  STSAFE-A120 Host key provisioning example                                   -
 ----------------------------------------------------------------------------------------------------------------
 -
 - description :
 - This examples illustrates host keys provisioning of a target STSAFE-A120 device
 -
 ----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120

 - Host mac key to write :

  0x00 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88 0x99 0xAA 0xBB 0xCC 0xDD 0xEE 0xFF

 - Host cipher key to write :

  0x01 0x23 0x45 0x67 0x89 0xAB 0xCD 0xEF 0x01 0x23 0x45 0x67 0x89 0xAB 0xCD 0xEF

 - Host key slot provisioning control fields :

 --------------+--------------+-----------+-------------------+----------------------------
  Change right | Re-provision | Plaintext | Wrapped anonymous | Wrapped authentication key
 --------------+--------------+-----------+-------------------+----------------------------
      YES      |     NO       |    YES    |        NO         |            0xFF
 --------------+--------------+-----------+-------------------+----------------------------

 +--------------------------------------------------------------------------------------------------------------+
 |                                                                                                              |
 |                                           /!\ W A R N I N G /!\                                              |
 |                                                                                                              |
 | The program will write test keys in the target STSAFE-A120 and enable re-provisioning                        |
 | It will add two following constraints:                                                                       |
 |    - The host keys slot will be opened to re-provisioning forever and this id not be suitable for production |
 |    - The target STSAFE-A120 will no longer support stse_host_key_provisioning_wrapped_authenticated          |
 |                                                                                                              |
 | Press any key to continue and write the host keys :                                                          |
 |                                                                                                              |
 +--------------------------------------------------------------------------------------------------------------+

 - Open host key slot to re-provisioning

 - stse_host_key_provisioning : PASS

```

Applicative scenario : 

- Restrict host key slot provisioning
- Host key provisioning plaintext