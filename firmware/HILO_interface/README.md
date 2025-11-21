![Project's banner](../../images/HILO_banner.png)

## 1.0 Files

| **Files and dirs** | **Description**                                                               |
|--------------------|-------------------------------------------------------------------------------|
| CMakeLists.txt     | It contains the rules you need to build the firmware or the integration tests |
| components         | All used by firmware libraries are stored in a sub-folder of this one         |
| main               | It contains the firmware and integration tests                                |
| [build]            | Temporary folder used to buid the code                                        |
| [CMakeLists.conf]  | Building process' configuration file                                          |
| configUtils.cmake  | CMake functions library                                                       |
| sdkconfig          | The ESP-IDF's configuration file                                              |

## 2.0 Description
This component provides the HILO device's interface feature. As you can read in the diagram in the main README file, this interface
is used by the test client software, and accepts data/commands to configure, start, manage the test process.

### 2.1 Hardware considerations
The MCU used to implement the HILO's interface is the 
[ESP32 WROOM-32-UE](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf).
It is a cheap device but its characteristics allows it to perform all the required features, easily. The followings are the main
ones:
- 448 KB ROM
- 520 KB SRAM
- WiFI support (802.11 b/g/n (802.11n up to 150 Mbps))
- Integrated crystal (40MHz)
- External antennna
- SPI and I2c BUS support

#### 2.2 Communication protocol
in order to obtain the most standard communication protocol possible, which also implements sophisticated access protection
mechanisms, the REST API protocol has been used for all HILO device's external communication.
	
###### 2.2.1 REST APIs for scheduled-test mode:
| REST API                          |  Description                                                                       |
|-----------------------------------|------------------------------------------------------------------------------------|
| **POST** <HILO>/test/set_recepit  | The test's recipit will be accepted, converted, and stored in the SRAM             |
| **POST** <HILO>/test/start        | If a recepit has been accepted and converted then this command will start the test |
| **POST** <HILO>/test/stop         | If a test is running then it will be stopped                                       |
| **GET**  <HILO>/test/get_status   | It returns the test status {IN_PROGRESS|COMPLETED|STOOPED}                         |
| **GET**  <HILO>/test/get_resuts   | It returns all data produced by the last executed test                             |

###### 2.2.2 JSONs for scheduled-test mode:
	<HILO>/test/set_recepit = {
		configuration: {
			//
			// I/O direction configuration
			//
			pin0:  {IN|OUT}, 
			pin1:  {IN|OUT},
			...
			pin15: {IN|OUT},

			// Frequency in Khz
			freq:  <n>           // 1 = 0.1 Khz
		},
		test_outputData[] : {
			type:  fixedTimePeriod,
			pin:   <0-15>,
			value: {high|low|swap},
			start: <milliseconds>,
			stop:  <milliseconds>
		}, {
			type:      squareWave,
			pin:       <0-15>,
			period:    <milliseconds>,
			dutyCycle: <0-100>,          // percent
			start:     <milliseconds>,
			stop:      <milliseconds>
		}, {
			type:   number,
			pins[]: <0-15>, <0-15>, <0-15>, <0-15>, <0-15>, <0-15>, <0-15>, <0-15>  // 8bits MSB --> LSB
			period: <milliseconds>,
			start:  <milliseconds>,
			stop:   <milliseconds>
		}, ...
	}

###### 2.2.3 REST APIs for interactive test mode:
| REST API                       |  Description                                             |
|--------------------------------|----------------------------------------------------------|
| **POST** <HILO>/intv/configure | It configures the bits direction ant the TCL's ip-address|
| **POST** <HILO>/intv/send_data | It force the HILO's output pins to change their status   |
| **GET**  <HILO>/intv/get_fuses | It returns the status of the fuses                       |

###### 2.2.4 JSONs for interactive-test mode:
	<HILO>/intv/configure = {
		pindir = <16bit hex-number>   // 0 = input; 1 = output
		streamaddr = <test-client ip-addr:socket>
	}
	<HILO>/intv/send_data = <16bit hex-number>


#### 2.3 SRAM content
The SRAMs are used just in scheduled-test-mode. This modality is based on a test-recipit composed by all the I/O changes and
the time of their performing, mainly. This data is translated by the HILO-interface into an IO-pins configurations sequence,
one for every time time period. Then the sequence is written into one of the two SRAM banks.

	
#### 2.4 Communication with HILO-engine
In order to exchange adata between the interface and the engine, they uses two different communication data buses. The first one
is used by the interface to send commands, and because it does not need to be very fast, the 12C bus has been selected for this
purpose. The second bus is used by the engine's MCU to send the input-pins data to the interface, and to allow fast data
transmission rating, the SPI bus has been selected.

###### 2.4.1 HILO-engine accepted commands in scheduled-test mode:

| **8bit numeric ID** | **Descriprion**       |
|---------------------|-----------------------|
|   HENG_TEST_START   | bla bla bla           |
|   HENG_TEST_STOP    | bla bla bla           |
| HENG_TEST_GETSTATUS | bla bla bla           |
		 
###### 2.4.2 HILO-engine accepted commands in interactive mode:

|  **8bit numeric ID**  | **Data field** |  **Descriprion**       |
|-----------------------|----------------|------------------------|
|    HENG_INTV_START    |     N/A        |                        |
|    HENG_INTV_STOP     |     N/A        |                        |
| HENG_INTV_SETPINSDIR  |  \<pins-dir>   |                        |
| HENG_INTV_SETPINSVAL  | \<pins-value>  |                        |

 
## 3.0 Buiding procedure
In order to build the code for ESP32 that belongs to this module, you need the
[ESP-IDS (5.4.x)](https://docs.espressif.com/projects/esp-idf/en/v5.4.2/esp32/get-started/index.html.) software framework.
Please, read the documentation to get the right procedure to build the code.

This firmware source code tree contains unit-tests and integration tests too. The **unit-tests** cover the components' code, not the
main. To build these tests find the "test" folder into the proper component home (component/\<component-name>) and follow the
instructions decribed in the README or Makefile file storeed in tht path. 
To execute the **integration tests**, you need to build the test for the target and also upload the test-firmware into the target. So,
in order to select wich firmware (test<n>.c, ...prod.c) you want to use you have to set the Makefike.conf file properly.
For further information, please, read the documentation section in the CMakeLists.txt file.


