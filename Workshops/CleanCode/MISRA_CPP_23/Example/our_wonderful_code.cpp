// How many MISRA violations?
#define MAX_SENSORS 10

class SensorArray {
    int* data;
    int size;

public:
    SensorArray(int s) {
        size = s;
        data = new int[size];
    }

    int getValue(int index) {
        return data[index];
    }

    void process() {
        int result;
        for (int i = 0; i <= size; i++) {
            if (data[i] < 0)
                goto error;
            result = data[i] * 2;
        }
        return;

    error:
        result = -1;
    }
};

void main() {
    SensorArray sensors(MAX_SENSORS);
    sensors.process();
}
