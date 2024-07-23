/*
 * Display the message "How many hours did you work?"
 * Input hours
 * Display the message "How much do you get paid per hour?"
 * Input rate
 * Calculate the pay due as the number of hours worked multiplied by the pay
 * rate If the number of hours is less than 10, add a bonus equal to their pay
 * rate for one hour If the number of hours is greater than 100, add a bonus
 * equal to their pay rate for five hours Display the pay due to the user
 */

#include<stdio.h>

int calculatePay(int hours, int rate);

int main() {
    printf("How many hours did you work?\n");
    int hours = 0;
    scanf("%d",&hours);

    printf("How much do you get paid per hour?\n");
    int rate = 0;
    scanf("%d", &rate);

    int pay = calculatePay(hours, rate);
    printf("Pay due: %d \n", pay);
}

int calculatePay(int hours, int rate) {
    if (hours < 10) {
        return (hours + 1) * rate;
    }

    if (hours > 100) {
        return (hours + 5) * rate;
    }

    return hours * rate;
}
