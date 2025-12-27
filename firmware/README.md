
![Project's banner](../images/HILO_banner.png)

## 1.0 Files

| **Files and dirs** | **Description**                                                            |
|--------------------|----------------------------------------------------------------------------|
| HILO_engine        | The real-time component that performs the scheduled test                   |
| HILO_interface     | The component that interface the HILO's engine with the test client system |

## 2.0 Description
In oder to get a more comfortable and an high performave instrument, the HILO device uses more MCUs.
This folder contains the firmwares for all used MCUs.
	
### 2.1 Dataflow description:
Please, consider the logic scheme stored in the ../PCB/README file.
The HILO can function in two different ways: **scheduled-mode** and **interactive-mode**.

- In the first one, the user have to prepare a test's recipit and send it via REST API to the HILO_interface component.
Then it will instructs the HILO_engine on the step iw will have to follow to perform the test correctly. At the end of
the test, the software-client will retrive all resulted data, and the user will be able to evaluate the the DUT's
behaviohr.
	
![Time sequence diagram for the **scheduled mode**](images/overviewSequenceDiagram.png)

- In the interactive-mode, the user can change the output-pins value via REST API, dinamically. All data on the monitored
input pins will be continuously reported via TCP/IP stream. So, the user will be able to evaluate the DUT behavior
modifying the DUT's input and checking for the DUT's output, dinamically. Because the DUT's inputs change manually, the
HILO's sample rating is set to 1Khz.

![Time sequence diagram for the **interactive mode**](images/overviewSequenceDiagram_intvMode.png)
