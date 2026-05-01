#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Vehicle {
private:
    float speed;
    float acceleration;
    float brake_force;
    int lane;

public:
    Vehicle(float spd = 0, int ln = 1) {
        speed = spd;
        acceleration = 2;
        brake_force = 5;
        lane = ln;
    }

    void accelerate() { speed += acceleration; }

    void brake() {
        speed -= brake_force;
        if (speed < 0) speed = 0;
    }

    void emergencyBrake() {
        speed -= 15;
        if (speed < 0) speed = 0;
    }

    void aiBehavior() {
        speed += (rand() % 3 - 1);
        if (speed < 5) speed = 5;
        if (speed > 25) speed = 25;
    }

    float getSpeed() { return speed; }
    float getSpeedMS() { return speed / 3.6; }
    int getLane() { return lane; }
};

class TrafficSimulation {
private:
    Vehicle player;
    Vehicle car1;
    Vehicle car2;

    float distance1;
    float distance2;
    float reaction_time;

    ofstream logFile;

public:
    TrafficSimulation() {
        player = Vehicle(0, 1);
        car1 = Vehicle(15, 1);
        car2 = Vehicle(20, 2);

        distance1 = 40;
        distance2 = 60;

        reaction_time = 1.5;

        logFile.open("simulation_log.txt");
    }

    void logData(float time, float d1, float d2, float speed) {
        logFile << time << "," << d1 << "," << d2 << "," << speed << endl;
    }

    void run() {

        char input;
        float time = 0;

        while (true) {

            car1.aiBehavior();
            car2.aiBehavior();

            distance1 += car1.getSpeedMS();
            distance2 += car2.getSpeedMS();

            distance1 -= player.getSpeedMS();
            distance2 -= player.getSpeedMS();

            if (distance1 < 0) distance1 = 0;
            if (distance2 < 0) distance2 = 0;

            float speed_ms = player.getSpeedMS();

            float safe_distance =
                (speed_ms * reaction_time) +
                (speed_ms * speed_ms) / 10;

            cout << "\n----------------------------\n";
            cout << "Time: " << time << " sec\n";
            cout << "Player Speed: " << player.getSpeed() << "\n";
            cout << "Distance1: " << distance1 << "\n";
            cout << "Distance2: " << distance2 << "\n";
            cout << "Safe Distance: " << safe_distance << "\n";

            if (distance1 < safe_distance || distance2 < safe_distance)
                cout << "[RISK]\n";
            else
                cout << "[SAFE]\n";

            logData(time, distance1, distance2, player.getSpeed());

            cout << "\n[a]=accel [b]=brake [e]=emergency [q]=quit\n";
            cin >> input;

            if (input == 'a') player.accelerate();
            else if (input == 'b') player.brake();
            else if (input == 'e') player.emergencyBrake();
            else if (input == 'q') break;

            if (distance1 <= 0 || distance2 <= 0) {
                cout << "\nCOLLISION OCCURRED!\n";
                break;
            }

            time += 1;
        }

        logFile.close();
    }
};

int main() {
    TrafficSimulation sim;
    sim.run();
    return 0;
}