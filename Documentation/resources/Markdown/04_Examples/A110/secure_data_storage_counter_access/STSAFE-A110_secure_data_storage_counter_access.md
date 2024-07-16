# STSAFE-A110_secure_data_storage_counter_accesses{#STSAFE-A110_secure_data_storage_counter_accesses}

This example illustrates how to makes use of the STSE data storage APIs by performing following accesses/commands to the target STSAFE-A110 device :
- Query STSAFE-A total partition count
- Query STSAFE-A partitions information
- Read STSAFE-A counter zone 5
- Update STSAFE-A counter zone 5

![STSAFE-A110_secure_data_storage_counter_access_example](./secure_data_storage_counter_access_example.png)

*Note zone IDs used in this example are aligned with STSAFE-A110 SPL03 personalization. Accesses parameters must be adapted for other device personalization.* 

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A110_secure_data_storage_zone_access.png} "STSAFE-A110_Device_authentication Example flowchart" width=5cm

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
	
	:Print data partition record table]
	
	:ret = <b>stse_data_storage_read_counter</b> (
	\tzone 5 , 
	\toffset 0 , 
	\tsize of associated buffer
	)|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif
	
	:Print counter value and associated data]
	
	:Randomize associated data buffer]
	
	:ret = <b>stse_data_storage_decrement_counter</b> (
	\tzone 5 , 
	\amount 1,
	\toffset 0 , 
	\tassociated data buffer,
	\tsize of associated buffer
	)|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif
	:Print new counter value ]
	
	:ret = <b>stse_data_storage_read_counter</b> (
	\tzone 5 , 
	\toffset 0 , 
	\tsize of associated buffer
	)|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif
	
	:Print counter value and associated data]
	
	while (while(1) )
	end while
	-[hidden]->
	detach
@enduml

API used in the example :

- \ref stse_init
- \ref stse_data_storage_read_counter
- \ref stse_data_storage_decrement_counter


When executed following logs can be seen on the host attached computer terminal window \n


```

----------------------------------------------------------------------------------------------------------------
-                             STSAFE-A110 secure data storage counter access example                           -
----------------------------------------------------------------------------------------------------------------
-
- description :
- This examples illustrates how to makes use of the STSAFE-A data storage APIs by performing following
- accesses/commands to the target STSAFE device
-          o Query STSAFE-A total partition count
-          o Query STSAFE-A partitions information
-          o Read STSAFE-A counter zone 5
-          o Update STSAFE-A counter zone 5
-
- Note : zone IDs used in this example are aligned with STSAFE-A110 SPL03 personalization
-        Accesses parameters must be adapted for other device personalization
-
----------------------------------------------------------------------------------------------------------------

- Initialize peripheral STSAFE :

 - stsafe_data_storage_read_zone ( zone : 05 ; length : 16)
         o Associated Data :
  0x3F 0xF0 0xB2 0x51 0xBA 0xA1 0x51 0xFA 0x72 0x04 0xC3 0x21 0x90 0x03 0xA8 0x21
         o Counter Value : 499991

 - stsafe_data_storage_read_zone ( zone : 05 ; length : 16)
         o New Associated Data :
  0x7D 0x32 0x09 0x10 0x6F 0x65 0x31 0x3A 0x34 0xDF 0xBF 0xF3 0x38 0x36 0xCE 0xE8
         o New Counter Value : 499990

 - stsafe_data_storage_read_zone ( zone : 01 ; length : 100)
         o Associated Data :
  0x7D 0x32 0x09 0x10 0x6F 0x65 0x31 0x3A 0x34 0xDF 0xBF 0xF3 0x38 0x36 0xCE 0xE8
         o Counter Value : 499990

```


Applicative scenario : 

- Accessory/Consumable usage monitoring 
- Accessory/Consumable Life/Cycle monitoring 