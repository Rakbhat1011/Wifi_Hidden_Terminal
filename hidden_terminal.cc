#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"

  
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RtsCtsExample");

void RunSimulation(bool rtsCtsEnabled, std::string wifiManager) 
{
  // Set up some default values for the simulation
  
  std::string tr_name("hidden-terminal");
  double simulationTime = 10.0; // seconds
 
  if (rtsCtsEnabled)

    {
      
      Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("5"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("50"));


    }

  else

    {
       Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2000"));

    }

  // Create nodes
  NodeContainer nodes;
  nodes.Create (3);

 
  // 4. Create & setup wifi channel
YansWifiChannelHelper wifiChannel;
wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
wifiChannel.AddPropagationLoss("ns3::FixedRssLossModel", "Rss", DoubleValue(-70.0));

// 5. Install wireless devices
WifiHelper wifi;
wifi.SetStandard(WIFI_STANDARD_80211b);
wifi.SetRemoteStationManager("ns3::" + wifiManager + "WifiManager");

YansWifiPhyHelper wifiPhy;
wifiPhy.SetChannel(wifiChannel.Create());
wifiPhy.Set("ChannelNumber", UintegerValue(1));

WifiMacHelper wifiMac;
wifiMac.SetType("ns3::AdhocWifiMac");

// Create and install wifi devices
NetDeviceContainer devices;
devices = wifi.Install(wifiPhy, wifiMac, nodes);
wifiPhy.Set("TxPowerStart", DoubleValue(20.0));
wifiPhy.Set("TxPowerEnd", DoubleValue(20.0));
wifiPhy.Set("RxSensitivity", DoubleValue(-94.0));



  // Set up mobility models
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  positionAlloc->Add (Vector (10.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);


InternetStackHelper internet;
  internet.Install (nodes);
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.0.0.0");
  /*InterfaceContainer interfaces;
interfaces = */
ipv4.Assign (devices);

Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

AsciiTraceHelper ascii;
MobilityHelper::EnableAsciiAll
(ascii.CreateFileStream (tr_name + ".tr"));
wifiPhy.EnableAsciiAll(ascii.CreateFileStream("hiddden.tr"));

FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  
  // Set up applications
  uint16_t port = 9; // well-known echo port number
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address ("10.0.0.2"), port));
  onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onOffHelper.SetAttribute ("DataRate", StringValue ("1Mbps"));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1024));
  onOffHelper.SetAttribute ("MaxBytes", UintegerValue (0));

  ApplicationContainer onOffApps;
  onOffApps = onOffHelper.Install (nodes.Get (0));
  onOffApps.Start (Seconds (1.0));
  onOffApps.Stop (Seconds (simulationTime));
  
  onOffApps = onOffHelper.Install (nodes.Get (2));
  onOffApps.Start (Seconds (1.2));
  onOffApps.Stop (Seconds (simulationTime));
  
    // Set up packet sink
  UdpServerHelper udpServer (port);
  ApplicationContainer sinkApps = udpServer.Install (nodes.Get (2));
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (simulationTime));

  // Enable pcap tracing
  wifiPhy.EnablePcap ("rts-cts-example", devices);

  // Set up simulation time and run the simulation
  Simulator::Stop (Seconds (simulationTime));
  AnimationInterface anim ("Waheguruji.xml");
  Simulator::Run ();
  monitor->CheckForLostPackets ();

Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

// Print per-flow statistics
std::cout << "Per-flow statistics:" << std::endl;
for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
{
 Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
  std::cout << "------------------------------------------------\n";
  std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
  std::cout << "  Transmitted Packets: " << i->second.txPackets << "\n";
  std::cout << "  Received Packets: " << i->second.rxPackets << "\n";
  std::cout << "  Lost Packets: " << i->second.lostPackets << "\n";
  std::cout << "  Transmitted Bytes: " << i->second.txBytes << "\n";
  std::cout << "  Received Bytes: " << i->second.rxBytes << "\n";
  std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ()) / 1000000.0 << " Mbps\n";
  
  
  /*f (i->first > 2)
        {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
          std::cout << "Flow " << i->first - 2 << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
          std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
          std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
          std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
        }*/
    }

Simulator::Destroy ();
}

int main(int argc,char **argv)
 {
  std::string wifiManager ("Arf");
  CommandLine cmd (__FILE__);
  cmd.AddValue ("wifiManager", "Set wifi rate manager", wifiManager);
  cmd.Parse (argc, argv);
  std::cout << "*****************************************************************\n";
  std::cout << "Hidden station experiment with RTS/CTS disabled:\n" << std::flush;
  RunSimulation (true, wifiManager);
  std::cout << "*****************************************************************\n";
  std::cout << "Hidden station experiment with RTS/CTS enabled:\n";
  RunSimulation(false, wifiManager);




  

  return 0;
}