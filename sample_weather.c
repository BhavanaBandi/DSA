#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROADS 10
#define ROAD_CAPACITY 100
#define MAX_SIGNALS 10

typedef enum {
    CAR,
    TRUCK,
    MOTORBIKE
} VehicleType;

typedef struct {
    VehicleType type;
    int size;
} Vehicle;

typedef struct {
    int cycleDuration;  // in seconds
    int trafficVolume;  // an arbitrary index representing traffic volume
    // Add more parameters as needed for calculation
} SignalData;

int trafficData[MAX_ROADS][MAX_ROADS] = {0};

void addRoad(int source, int destination, int traffic) {
    trafficData[source][destination] = traffic;
    trafficData[destination][source] = traffic;
}

void displayTraffic() {
    printf("Traffic Data:\n");
    printf("Roads\tTraffic\n");
    for (int i = 0; i < MAX_ROADS; i++) {
        for (int j = i + 1; j < MAX_ROADS; j++) {
            if (trafficData[i][j] > 0) {
                printf("%d - %d\t%d\n", i, j, trafficData[i][j]);
            }
        }
    }
}

int minDistance(int dist[], bool visited[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < MAX_ROADS; v++) {
        if (visited[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstraShortestPath(int src, int dest) {
    int dist[MAX_ROADS];
    bool visited[MAX_ROADS];

    for (int i = 0; i < MAX_ROADS; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < MAX_ROADS - 1; count++) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        for (int v = 0; v < MAX_ROADS; v++) {
            if (!visited[v] && trafficData[u][v] && dist[u] != INT_MAX && dist[u] + trafficData[u][v] < dist[v]) {
                dist[v] = dist[u] + trafficData[u][v];
            }
        }
    }

    printf("No. of vehicles present on this particular road from %d to %d: %d\n", src, dest, dist[dest]);
}

void simulateTrafficUpdate(int userSource, int userDestination) {
    srand(time(NULL));

    // Only update the user's chosen road
    int increment = rand() % 10;
    trafficData[userSource][userDestination] += increment;
    trafficData[userDestination][userSource] += increment;
}

void simulateTrafficFlow() {
    srand(time(NULL));
    for (int i = 0; i < MAX_ROADS; i++) {
        for (int j = i + 1; j < MAX_ROADS; j++) {
            if (trafficData[i][j] > 0) {
                int currentTraffic = trafficData[i][j];
                int capacityLeft = ROAD_CAPACITY - currentTraffic;
                if (capacityLeft > 0) {
                    int increment = rand() % (capacityLeft + 1);
                    trafficData[i][j] += increment;
                    trafficData[j][i] += increment;
                }
            }
        }
    }
}

void simulateTrafficTimeOfDay(int hour) {
    srand(time(NULL));
    for (int i = 0; i < MAX_ROADS; i++) {
        for (int j = i + 1; j < MAX_ROADS; j++) {
            if (trafficData[i][j] > 0) {
                int baseTraffic = trafficData[i][j];
                if (hour >= 7 && hour <= 9) {
                    trafficData[i][j] += baseTraffic * 0.3;
                    trafficData[j][i] += baseTraffic * 0.3;
                } else if (hour >= 16 && hour <= 18) {
                    trafficData[i][j] += baseTraffic * 0.4;
                    trafficData[j][i] += baseTraffic * 0.4;
                }
            }
        }
    }
}

void simulateTrafficWeather(bool isRaining, bool isSnowing, bool isDrizzling) {
    srand(time(NULL));
    double trafficMultiplier = 1.0;
    if (isRaining) {
        trafficMultiplier = 1.5;
    } else if (isSnowing) {
        trafficMultiplier = 2.0;
    } else if (isDrizzling) {
        trafficMultiplier = 1.2;
    }
    for (int i = 0; i < MAX_ROADS; i++) {
        for (int j = i + 1; j < MAX_ROADS; j++) {
            if (trafficData[i][j] > 0) {
                trafficData[i][j] *= trafficMultiplier;
                trafficData[j][i] *= trafficMultiplier;
            }
        }
    }
}

void findShortestPath(int source, int destination) {
    dijkstraShortestPath(source, destination);
}

int estimateSignalWaitTime(int signalID, SignalData signalData[MAX_SIGNALS]) {
    if (signalID < 0 || signalID >= MAX_SIGNALS) {
        printf("Invalid signal ID.\n");
        return -1; // Return an error code or handle invalid signal IDs
    }

    int score = signalData[signalID].cycleDuration * signalData[signalID].trafficVolume;
    // Add more factors and adjust the calculation based on your scenario

    int estimatedWaitTime = score * 2; // Placeholder formula for illustration purposes

    // Introduce randomness by adding a random value between 0 and 30 seconds
    int randomOffset = rand() % 31;
    estimatedWaitTime += randomOffset;

    return estimatedWaitTime;
}


int main() {
    FILE *file;
    file = fopen("roads_input.txt", "r");

    if (file == NULL) {
        perror("Error opening the file.\n");
        return -1;
    }

    
    int source, destination, traffic;
    while (fscanf(file, "%d %d %d", &source, &destination, &traffic) != EOF) {
        addRoad(source, destination, traffic);
    }

    fclose(file);

    displayTraffic();

    printf("\nEnter source road: ");
    scanf("%d", &source);
    printf("Enter destination road: ");
    scanf("%d", &destination);

    if (source < 0 || source >= MAX_ROADS || destination < 0 || destination >= MAX_ROADS) {
        printf("Invalid source or destination road.\n");
        return -1;
    }

    findShortestPath(source, destination);

    // Move simulateTrafficUpdate here
    simulateTrafficUpdate(source, destination);

    simulateTrafficFlow();

    int currentHour = 8;
    simulateTrafficTimeOfDay(currentHour);

    bool isRaining, isSnowing, isDrizzling;
    printf("Is it raining? (1 for Yes, 0 for No): ");
    scanf("%d", &isRaining);
    printf("Is it snowing? (1 for Yes, 0 for No): ");
    scanf("%d", &isSnowing);
    printf("Is it drizzling? (1 for Yes, 0 for No): ");
    scanf("%d", &isDrizzling);

     int userSignalID;
    printf("Enter signal ID: ");
    scanf("%d", &userSignalID);

    // Initialize signalData array
    SignalData signalData[MAX_SIGNALS] = {0};

    // Populate signalData array with characteristics
    signalData[0].cycleDuration = 60;  // Signal 0 has a cycle duration of 60 seconds
    signalData[0].trafficVolume = 3;   // Signal 0 has a traffic volume index of 3 (example)

    signalData[1].cycleDuration = 45;  // Signal 1 has a cycle duration of 45 seconds
    signalData[1].trafficVolume = 5;   // Signal 1 has a traffic volume index of 5 (example)

    // Add data for other signals...

    int waitTime = estimateSignalWaitTime(userSignalID, signalData);

    // Display estimated wait time
    printf("\nEstimated Wait Time at Signal %d: %d seconds\n", userSignalID, waitTime);

    return 0;
}

