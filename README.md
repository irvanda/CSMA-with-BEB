# CSMA-with-BEB
Simulation of CSMA/CA with BEB (Binary Exponential Backoff) algorithm. 

# assignment 1:**

**Assignment:**
When there are multi users in a BSS (one AP and multiple UEs), simulate the throughput performance for uplink random access transmission by using C simulator implemented for yourself.

**Simulation Environments:**
-	Topology: one AP in center and many UEs within the range of AP in uniformly distributed manner. (UEs from 10 to 100 increased by 10)
-	CSMA/CA with BEB algorithm for random access from multi UEs to AP transmission. 
-	Window size ranges from 8 slots to 256 slots. Once the transmission is successful, window size goes back to its minimum, 8 slots. If the unsuccessful transmission happens after the window size reaches to its maximum, 256 slots, the window size will be fixed to its value until transmitting the packet successfully.
-	1ms slot 
-	Ignore DIFS, AIFS, and ACK.
-	Assume the perfect channel (no errors during transmission)
-	Ignore the effect on user location (all has same channel characteristics)
-	Packet size is fixed with 10 slot-lengths. Every UE has packets to transmit (the transmission buffer is fully occupied).
-	Simulation time 100 seconds (ignore the first 10 seconds for performance evaluation)
-	Iteration is 100 times, so that the results are averaged. 

**Performance measure:**
-	Throughput: x-axis is the number of users and y-axis is the throughput in the sense of the number of successfully transmitted packets.

**Simulation procedure:**
1.	Generate users. Initiate all parameters related to user’s statistics.
2.	Start simulation from slot 0 (0 second)  all users select initial backoff value within 0~7
3.	Find users who get 0 for backoff value and see if collision occurs. In case collision, the corresponding users extend the window size double and select a new backoff value within the range randomly. Otherwise its transmission is successful and the successful transmission needs to be counted.
4.	Case 1: Success or collision in 3, move slots up to 10 slots and reduce all users’ backoff value by 1. Follow the step 3. Case 2: No UEs in 3. Move 1 slot and reduce all users’ backoff value by 1. Go back to step 3.


# Assignment 2:

**Assignment:**
Voice Traffic generator and its controller need to be added.

**Simulation Environments:**
-	Topology: one AP in center and many UEs within the range of AP in uniformly distributed manner. (UEs from 10 to 100 increased by 10)
-	CSMA/CA with BEB algorithm for random access from multi UEs to AP transmission. 
-	Window size ranges from 8 slots to 256 slots. Once the transmission is successful, window size goes back to its minimum, 8 slots. If the unsuccessful transmission happens after the window size reaches to its maximum, 256 slots, the window size will be fixed to its value until transmitting the packet successfully.
-	1ms slot 
-	Ignore DIFS, AIFS, and ACK.
-	Assume the perfect channel (no errors during transmission)
-	Ignore the effect on user location (all has same channel characteristics)
-	Packet size is fixed with 10 slot-lengths. 
-	Simulation time 100 seconds 
-	Iteration is 100 times, so that the results are averaged. 
Performance measure:
-	PER (Packet error rate): x-axis is the number of users and y-axis is an average per for all voice users.
-	Maximum tolerable waiting time for voice traffic is 200ms. The packets exceeding 200ms waiting time are dropped from buffer and count +1 for packet loss.

**Simulation procedure:**
1.	Generate users. Initiate all parameters related to user’s statistics.
2.	Start simulation from slot 0 (0 second)  all users select initial backoff value within 0~7
3.	Find users who get 0 for backoff value and see if collision occurs. In case collision, the corresponding users extend the window size double and select a new backoff value within the range randomly. Otherwise its transmission is successful and the successful transmission needs to be counted.
4.	Case 1: Success or collision in 3, move slots up to 10 slots and reduce all users’ backoff value by 1. Follow the step 3. Case 2: No UEs in 3. Move 1 slot and reduce all users’ backoff value by 1. Go back to step 3.

**Voice traffic model:** 
-	Voice traffic generates 8000bits per second only when it is in talkspurts state.
-	Every 160bits are packed a packet.
-	Generated packet is buffered in queue and waits for transmission.
-	When a packet is generated, a timer starts from 0. If the timer reaches 200ms, the packet is useless.
-	We assumed that a packet occupies 10 slots in channel.


# Assignment 3:

**Assignment:**
PER performance evaluation for real time traffics in WLAN
Simulation Environments:
-	Topology: one AP in center and many UEs within the range of AP in uniformly distributed manner. 
-	For the population in the system, the number of voice users steps up by 10 from 10 up to 1000 when the number of video users is always kept in 10.
-	CSMA/CA with BEB algorithm for random access from multi UEs to AP transmission. 
-	Window size ranges from 8 slots to 1024 slots. Once the transmission is successful, window size goes back to its minimum, 8 slots. If the unsuccessful transmission happens after the window size reaches to its maximum, 1024 slots, the window size will be fixed to its value until transmitting the packet successfully.
-	10us slot 
-	Ignore DIFS, AIFS, and ACK.
-	Assume the perfect channel (no errors during transmission)
-	Ignore the effect on user location (all has same channel characteristics)
-	Packet size is fixed for voice traffic and variable for video traffic. Channel data rate (source data rate in this case) is assumed 8Mbps. Zero padding may be issued to make slot based resolution.
-	Simulation time 100 seconds 
-	Iteration is 100 times, so that the results are averaged. 

**Performance measure:**
-	PER (Packet error rate) for voice as well as video: the number of dropped packets/the total number of generated packets. 
-	Maximum tolerable waiting time for voice and video traffics are 200ms. The packets exceeding 200ms waiting time are dropped from buffer and count +1 for packet loss.

**Simulation procedure:**
1.	Generate users. Initiate all parameters related to user’s statistics.
2.	Start simulation from slot 0 (0 second)  all users select initial backoff value within 0~7
3.	Find users who get 0 for backoff value and see if collision occurs. In case collision, the corresponding users extend the window size double and select a new backoff value within the range randomly. Otherwise its transmission is successful and the successful transmission needs to be counted.
4.	Case 1: Success or collision in 3, move slots up to 10 slots and reduce all users’ backoff value by 1. Follow the step 3. Case 2: No UEs in 3. Move 1 slot and reduce all users’ backoff value by 1. Go back to step 3.

**video traffic model:**
Table Parameters of 32 kbps video streaming model.

| Information types |	Distribution |	Distribution parameters |
| --- | --- | --- |
|Inter-arrival time between the beginning of each video-frame |	Deterministic |	100 ms |
|Number of video packet in a video-frame	| Deterministic	| 8 |
|Video-packet size	| Truncated Pareto |	(Max.=125 bytes) k=20, α=1.1 |
|Inter-arrival time between video-packets in a video-frame |	Truncated Pareto | (Mean=6 ms, Max.=12.5 ms)	k=2.5, α=1.2 |
