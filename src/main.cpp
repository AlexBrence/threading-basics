#include "packet.h"
#include "packet2.h"
#include "packetprocessor.h"

#include <chrono>
#include <iostream>
#include <thread>


int main() 
{
  const int numOfPackets = 10'000;

  /* ------------- Processor that uses virtual function --------------- */
  PacketProcessor packetProcessor;

  auto startTime = std::chrono::high_resolution_clock::now();

  /* Create background thread */
  std::thread thread(&PacketProcessor::processPackets, &packetProcessor);

  /* Add packets */
  for (int i = 0; i < numOfPackets; i++) 
  {
    packetProcessor.addPacket(std::make_shared<Packet>(i));
  }

  /* Wait for packets to be processed */
  while (!packetProcessor.isIdle()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  /* Print out exception count */
  std::cout << "Exception count: " << packetProcessor.getExceptionCount() << std::endl;

  /* Abort */
  packetProcessor.abort();
  thread.join();

  auto endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> interfaceDurationVirtual = endTime - startTime;


  /* ------------- Processor that uses std::function --------------- */
  std::cout << "\nProcessing with callback function\n";
  PacketProcessor packetProcessor2;

  startTime = std::chrono::high_resolution_clock::now();

  /* Create background thread */
  std::thread thread2(&PacketProcessor::processPackets, &packetProcessor2);

  /* Add packets */
  for (int i = 0; i < numOfPackets; i++) 
  {
    packetProcessor2.addPacket(std::make_shared<Packet2>(i, []() {
      std::cout << "Executing callback function" << std::endl;
    }));
  }

  /* Wait for packets to be processed */
  while (!packetProcessor2.isIdle()) 
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  /* Print out exception count */
  std::cout << "Exception count: " << packetProcessor2.getExceptionCount() << std::endl;

  /* Abort */
  packetProcessor2.abort();
  thread2.join();

  endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> interfaceDurationFunction = endTime - startTime;

  /* Print out durations */
  std::cout << "Duration with virtual function: " << interfaceDurationVirtual.count() << " s\n"
    << "Duration with std::function: " << interfaceDurationFunction.count() << " s" << std::endl;


  return 0;
}
