# Dynamic Traffic Signal Management System

## **Description**
This project simulates a **dynamic traffic signal management system** for multiple lanes using **Linux C++**. It integrates real-world traffic scenarios such as **VIP vehicles, ambulances, fire trucks, and lane incidents** (accidents, road works, police chase). The system uses **priority-based lane selection, vehicle wait times, and traffic light control** to optimize traffic flow. Weather conditions affect traffic intensity, and the status of each lane is displayed in a **tabular format**. All outputs are logged to a **text file** for record-keeping and analysis.  
Additionally, when the simulation is ended by pressing **'q' and Enter**, a new log file is created that prints the **entire output history** for that session.

---

## **Features**
- 🚦 Multi-lane traffic simulation with **dynamic vehicle flow**.  
- 🚑 **Priority handling** for VIP, ambulance, and fire truck vehicles.  
- ⚠️ Random incidents (**accident, road work, police chase**) affecting lanes.  
- 🔴🟡🟢 **Traffic lights**: Red, Yellow, and Green with priority logic.  
- 🌦️ **Weather effects** impacting traffic intensity.  
- 📊 Tabular display of **lane status** in console.  
- 📝 Output **logging to a text file** for analysis.  
- 📂 On ending simulation with **'q' + Enter**, a new log file is generated containing the **complete output history**.  

---

## **Technologies Used**
- **Programming Language:** C++  
- **Libraries:** `<iostream>`, `<vector>`, `<thread>`, `<chrono>`, `<random>`  
- **Platform:** Console-based, cross-platform compatible  

---

## **Installation**
1. **Clone the repository:**
   ```bash
   git clone https://github.com/Rushikesh0864/Smart-Traffic-Light-System-Simulator.git
   ```

2. **Compile the code using g++:**
   ```bash
   g++ main.cpp -o traffic_sim
   ```

3. **Run the simulation:**
   Run on GDG Compiler for proper output.
   

---

## **Usage**
1. Enter the **number of lanes**.  
2. Specify if each lane has **VIP, ambulance, or firetruck vehicles**.  
3. Enter the **incident type** for each lane.  
4. The system will display a **dynamic table** of traffic lane statuses and save the outputs to **traffic_log.txt**.  
5. Press **'q' + Enter** to stop the simulation. A new log file will then be created containing the **complete session history**.  

---

## **Output**
- Real-time **lane status** displayed in console.  
- **Traffic statistics**, including vehicle count, wait times, priority reasons, and incidents.  
- All outputs logged to a file **traffic_log.txt**.  
- When simulation is ended with **'q' and  Enter**, a separate log file containing the **full session history** is generated.  

---

## **Future Enhancements**
- 🤖 **AI-based lane prediction** for smarter traffic flow.  
- 📡 Integration with **real-time sensors or IoT devices**.  
- 🖥️ **Graphical user interface** for better visualization.  

---

## **Author**
👤 **Rushikesh Doke**
