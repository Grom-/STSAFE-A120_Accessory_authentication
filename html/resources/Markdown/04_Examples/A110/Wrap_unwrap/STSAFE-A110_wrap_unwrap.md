# STSAFE-A110_wrap_unwrap{#STSAFE-A110_wrap_unwrap}

This example illustrates how how to wrap/unwrap payload using a target STSAFE-A110 device

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A110_wrap_unwrap.png} "STSAFE-A110_wrap_unwrap Example flowchart" width=5cm

	:MAIN;
	:Initialize Apps terminal (baudrate = 115200)]
	
	:Print example title and instructions]
	
	:ret = <b>stse_init</b>|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif

	:ret = <b>stsafea_generate_wrap_unwrap_key</b>|
	if((ret != STSE_OK) && (ret != STSE_ACCESS_CONDITION_NOT_SATISFIED)) then (yes)
		:Print ERROR]
		-[hidden]->
		detach
	(No) else if (ret != STSE_ACCESS_CONDITION_NOT_SATISFIED) then (Yes)
		:Print "New key provisioned"]
	else(No)
	endif
	
	:Print plain-text payload]

	:<b>stsafea_session_handler_allocate</b>]

	:ret = <b>stsafea_open_host_session</b>|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:ret = <b>stsafea_wrap_payload</b> (key 0 , plain-text payload) |
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:ret = <b>stsafea_wrap_payload</b> (key 0 , plain-text payload) |
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:Print wrapped payload]


	:ret = <b>stsafea_unwrap_payload</b> (key 0 , wrapped payload) |
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:Print un-wrapped payload]

	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

API and services used in the example :

- \ref stse_init
- \ref stsafea_generate_wrap_unwrap_key
- \ref stsafea_open_host_session
- \ref stsafea_wrap_payload
- \ref stsafea_unwrap_payload


When executed following logs can be seen on the host attached computer terminal window \n


```
----------------------------------------------------------------------------------------------------------------
-                                  STSAFE-A110 Wrap/Unwrap example                                             -
----------------------------------------------------------------------------------------------------------------
-
- description :
- This examples illustrates how to wrap/unwrap payload using a target STSAFE-A110 device
-
----------------------------------------------------------------------------------------------------------------
 - Plain-text buffer to be wrapped  :

  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

 - Wrapped buffer  :

  0x93 0x56 0xCC 0x9A 0x17 0x19 0x76 0xDA 0xEA 0x24 0xEA 0xDB 0x52 0x11 0xC4 0x46
  0x99 0xAB 0x77 0x6D 0x15 0x13 0x26 0x76

 - Un-wrapped buffer  :

  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

```

Applicative scenario : 

- storage of secret information on host
- pereparation of secret information to be store in STSAFE-A device 