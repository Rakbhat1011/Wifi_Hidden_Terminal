# Wifi_Hidden_Terminal
Project implementing the hidden terminal problem in WiFi networks

## <a name="1_Introduction"></a> Introduction

This project implements the Wifi Hidden Terminal scenario using NS3, a discrete-event network simulator. The hidden terminal problem occurs in wireless networks when a node is visible from a wireless access point (AP), but not from other nodes communicating with the AP, causing collisions. To mitigate this issue, we implement the Request-to-Send/Clear-to-Send (RTS/CTS) mechanism. This project includes a comparison of throughput under various conditions such as different loss models, data rates, and the use of RTS/CTS.

## <a name="2_Project Outcomes"></a> Project Outcomes

* Gain practical experience with NS3, configuring devices and simulating data traffic.
* Understand and implement RTS/CTS to mitigate the hidden terminal problem.
* Comparative Analysis: Analyze different network configurations and present results graphically.
* Performance Evaluation: Measure and interpret metrics like throughput, packet loss, and latency.
* Software Engineering Practices: Apply best practices in code design, documentation, and version control.

## <a name="3_Code Flow"></a> Code Flow

* Enable CTS/RTS: Configure the NS3 simulator to enable the RTS/CTS mechanism to reduce collisions in the hidden terminal scenario.
* Node Creation: Create multiple nodes that will represent the wireless stations and access points in the network.
* Create and Setup Channel: Establish a wireless channel with the desired physical characteristics and propagation loss model.
* Install Wireless Device: Attach wireless network devices to the nodes, configuring them with necessary MAC and PHY parameters.
* Create and Install Wifi Device: Use the NS3 WifiHelper to install and configure the Wifi devices on each node.
* Setup Mobility Model: Assign a mobility model to each node to simulate the movement and positioning of the wireless stations.
* Setup Application: Install applications on the nodes to generate traffic and simulate data transmission between the nodes.
* Setup Sink: Install a packet sink on the receiver nodes to collect incoming packets and measure throughput.
* Run Simulation: Execute the simulation for a specified duration, during which data transmission and reception events occur.
* Print Flow Statistics: After the simulation completes, collect and print statistics on network performance, including throughput, packet loss, and latency.

## <a name="3_Results"></a> Results

The results section includes a detailed analysis of the simulation output, comparing the throughput under different configurations:

* Loss Model: Evaluation of throughput performance with different wireless propagation loss models.
* Data Rate: Analysis of throughput variations with different data rates.
* RTS/CTS: Comparison of network performance with and without the RTS/CTS mechanism enabled.

Graphs and tables are provided to visually represent the impact of these factors on the network's throughput, demonstrating the effectiveness of RTS/CTS in mitigating the hidden terminal problem and improving overall network performance.
