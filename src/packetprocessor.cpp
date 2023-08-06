#include "packetprocessor.h"
#include <stdexcept>


PacketProcessor::PacketProcessor() 
  : m_isIdle(true), m_exceptionCount(0), m_abort(false) 
{}


/**
 * Adds packets to the queue
 * @param packet The packet to add to the queue
 */
void PacketProcessor::addPacket(std::shared_ptr<PacketBase> packet) 
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_packetQueue.push(packet);
  m_isIdle = false;
  m_condVar.notify_one();
}

/**
 * Checks if all packets are processed 
 */
bool PacketProcessor::isIdle() 
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_isIdle;
}

long long int PacketProcessor::getExceptionCount() 
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_exceptionCount;
}

/**
 * Aborts from processing packets
 */
void PacketProcessor::abort() 
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_abort = true;
  m_condVar.notify_one();
}

/**
 * Processes packets from the queue, invokes abort if set to true
 */
void PacketProcessor::processPackets() 
{
  while (true) 
  {
    std::unique_lock<std::mutex> lock(m_mutex);

    /* Continue only if queue is not empty or if abort is invoked */
    m_condVar.wait(lock, [this]{ return !m_packetQueue.empty() || m_abort; });

    if (m_abort) 
    {
      break;
    }

    std::shared_ptr<PacketBase> packet = m_packetQueue.front();
    m_packetQueue.pop();

    /* Doesn't need to be locked for now since we are processing packets */
    lock.unlock();

    /* Try to process packet, if exception is thrown increase our m_exceptionCount counter */
    try 
    {
      packet->process();
    }
    catch (const std::runtime_error& rErr) 
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_exceptionCount++;
      std::cout << "Runtime error\n";
    }

    /* Lock again since we are checking for member variable value and then modifying others */
    lock.lock();
    if (m_packetQueue.empty()) 
    {
      m_isIdle = true;
      m_condVar.notify_all();
    }
  }
}
