#include <iostream>

int round(float num) {
    bool positive = true;
    if(num < 0){
        positive = false;
        num = -num;
    }

    int ent = (int)num;
    float buff = num - ent;

    if (buff < 0.5)
        return (positive ? ent:(-ent));
    else 
        return (positive ? (ent+1):(-(ent+1)));
}

int main() {

	float num = -3.5;
	std::cout << round(num) << '\n';
	return 0;
}
