# Strategy

In this stage of the HFT project, my client uses a simple momentum-based trading rule, `void detect_momentum(...)`. 
Each time the server sends a new message in the format `price_id,price`, the client parses 
the message and extracts both the price ID and the price value. The client then stores the 
most recent three prices in a `std::deque<float>`, which acts as a rolling price window.

Once three prices are available, the client checks whether the sequence is strictly increasing 
or strictly decreasing. If the prices move upward consecutively, the client interprets this as 
upward momentum; if they move downward consecutively, it interprets this as downward momentum. 
In either case, the client sends an order using the current `price_id` after a short randomized 
reaction delay. If no clear momentum is detected, the client ignores the update. This strategy 
is intentionally simple so that the trading logic is easy to test, explain, and extend in later
stages of the project.

Output Sample
```angular2html
Enter your client name: Moxiao
✅ Connected to server at 127.0.0.1:12345
📥 Received price ID: 1, Value: 124.9
📥 Received price ID: 2, Value: 107.3
📥 Received price ID: 3, Value: 165.8
No momentum. Ignoring price ID 3
📥 Received price ID: 4, Value: 193
Momentum up! Sending order for price ID 4
📥 Received price ID: 5, Value: 127.2
No momentum. Ignoring price ID 5
📥 Received price ID: 6, Value: 154.4
No momentum. Ignoring price ID 6
📥 Received price ID: 7, Value: 187.8
Momentum up! Sending order for price ID 7
📥 Received price ID: 8, Value: 192.3
Momentum up! Sending order for price ID 8
📥 Received price ID: 9, Value: 170.9
No momentum. Ignoring price ID 9
📥 Received price ID: 10, Value: 144
Momentum down! Sending order for price ID 10



```