#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include <stdlib.h>
#define max 1000;
/**
 * main.c
 */

int direct = 0;
int phase = 0;
int ncount = 0;

int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;
int sensor4 = 0;
int sensor5 = 0;
int sensor6 = 0;
int sensor7 = 0;
int sensor8 = 0;

void LED() {
    P2->SEL0 &= ~0x07;
    P2->SEL1 &= ~0x07;

    P1->SEL0 &= ~0x01;
    P1->SEL1 &= ~0x01;

    P2->DIR |= 0x07;
    P1->DIR |= 0x01;

    P2->OUT &= ~0x07;
    P1->OUT &= ~0x01;
}
void SysTick_Init(void) {
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL = 0x00000005;
}

void SysTick_Wait1ms() {
    SysTick->LOAD = 0x0000BB80; // BB80 == 48000.;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0) {};
}

void SysTick_Wait1s() {
    int i, count = 1000;
    for (i = 0; i < count; i++) {
        SysTick_Wait1ms();
    }
    printf("1s passed...\n");
}

void (*TimerA2Task)(void);
void TimerA2_Init(void(*task)(void),uint16_t period) {
    TimerA2Task = task;
    TIMER_A2->CTL = 0x0280;
    TIMER_A2->CCTL[0] = 0x0010;
    TIMER_A2->CCR[0] = (period - 1);
    TIMER_A2->EX0 = 0x0005;
    NVIC->IP[3] = (NVIC->IP[3]&0xFFFFFF00) | 0x00000040;
    NVIC->ISER[0] = 0x00001000;
    TIMER_A2->CTL |= 0x0014;
}

void TA2_0_IRQHandler(void) {
    TIMER_A2->CCTL[0] &= ~0x0001;
    (*TimerA2Task)();
}
uint32_t count;

void Task(){
    printf("interrupt occurs!\n");
}

void PWM_Init34(uint16_t period, uint16_t duty3, uint16_t duty4) {
    P2->DIR |= 0xC0;
    P2->SEL0 |= 0xC0;
    P2->SEL1 &= ~0xC0;

    TIMER_A0->CCTL[0] = 0x800;
    TIMER_A0->CCR[0] = period;

    TIMER_A0->EX0 = 0x0000;

    TIMER_A0->CCTL[3] = 0x0040;
    TIMER_A0->CCR[3] = duty3;
    TIMER_A0->CCTL[4] = 0x0040;
    TIMER_A0->CCR[4] = duty4;

    TIMER_A0->CTL = 0x02F0;
}

void PWM_Duty3(uint16_t duty3) {
    TIMER_A0->CCR[3] = duty3;
}

void PWM_Duty4(uint16_t duty4) {
    TIMER_A0->CCR[4] = duty4;
}

void Move(uint16_t leftDuty, uint16_t rightDuty) {
    P3->OUT |= 0xC0;
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);
}

void Left_Forward() {
    P5->OUT &= ~0x10;
}
void Left_Backward() {
    P5->OUT |= 0x10;
}
void Right_Forward() {
    P5->OUT &= ~0x20;
}
void Right_Backward() {
    P5->OUT |= 0x20;
}

void Sensor_Init() {
    // 0, 2, 4, 6 IR matter
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;  // GPIO
    P5->DIR |= 0x08;    // OUTPUT
    P5->OUT &= ~0x08;   // turn off 4 even IR LEDs

    // 1, 3, 5, 7 IR Emitter
    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;  // GPIO
    P9->DIR |= 0x04;    // OUTPUT
    P9->OUT &= ~0x04;   // turn off 4 odd IR LEDs

    // 0~7 IR sensor
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;  // GPIO
    P7->DIR &= ~0xFF;   // INPUT
}

void Motor_Init(void) {
    P3->SEL0 &= ~0xC0;
    P3->SEL1 &= ~0xC0;  // 1) configure nSLPR & nSLPL as GPIO
    P3->DIR |= 0xC0;    // 2) make nSLPR & nSLPL as output
    P3->OUT &= ~0xC0;   // 3) output LOW

    P5->SEL0 &= ~0x30;
    P5->SEL1 &= ~0x30;  // 1) configure nSLPR & nSLPL as GPIO
    P5->DIR |= 0x30;    // 2) make DIRL & DIRL as output
    P5->OUT &= ~0x30;   // 3) output LOW

    P2->SEL0 &= ~0xC0;  // 1) configure nSLPR & nSLPL as GPIO
    P2->SEL1 &= ~0xC0;
    P2->DIR |= 0xC0;    // 2) make PWMR & PWMR as output
    P2->OUT &= ~0xC0;   // 3) output LOW

    PWM_Init34(15000,0,0);
}

void Readysensor() {
    P5->OUT |= 0x08;
    P9->OUT |= 0x04;
    P7->DIR = 0xFF;
    P7->OUT = 0xFF;
    Clock_Delay1us(10);
    P7->DIR = 0x00;
    Clock_Delay1us(1000);

    sensor1 = P7->IN & 0x01;
    sensor2 = P7->IN & 0x02;
    sensor3 = P7->IN & 0x04;
    sensor4 = P7->IN & 0x08;
    sensor5 = P7->IN & 0x10;
    sensor6 = P7->IN & 0x20;
    sensor7 = P7->IN & 0x40;
    sensor8 = P7->IN & 0x80;
}

void Timer_A3_Capture_Init() {
    P10->SEL0 |= 0x30;
    P10->SEL1 &= ~0x30;
    P10->DIR &= ~0x30;

    TIMER_A3->CTL &= ~0x0030;
    TIMER_A3->CTL = 0x0200;

    TIMER_A3->CCTL[0] = 0x4910;
    TIMER_A3->CCTL[1] = 0x4910;
    TIMER_A3->EX0 &= ~0x0007;

    NVIC->IP[3] = (NVIC->IP[3]&0x0000FFFF) | 0x404000000;
    NVIC->ISER[0] = 0x0000C000;
    TIMER_A3->CTL |= 0x0024;
}

uint16_t first_left;
uint16_t first_right;

uint16_t period_left;
uint16_t period_right;

void TA3_0_IRQHandler(void) {
    TIMER_A3->CCTL[0] &= ~0x0001;
    period_right = TIMER_A3->CCR[0] - first_right;
    first_right = TIMER_A3->CCR[0];
}

void TA3_N_IRQHandler(void)
{
    TIMER_A3->CCTL[1] &= ~0x0001;
    count++;
}

void Turn_Left(int sp)
{
    Left_Backward();
    Right_Forward();
    Move(sp+300, sp+300);
    count = 0;
    while(1) {
        if (count > 90) {
            Move(0, 0);
            Clock_Delay1ms(300);
            direct++;
            direct = direct % 8;
            count = 0;
            break;
        }
    }
}

void Turn_Right(int sp)
{
    Left_Forward();
    Right_Backward();
    Move(sp+300, sp+300);
    count = 0;
    while(1) {
        if (count > 90) {
            Move(0, 0);
            Clock_Delay1ms(300);
            direct--;
            if (direct < 0)
                direct += 8;
            count = 0;
            break;
        }
    }
    Move(0,0);
}

void Turn_Left2(int sp, int cnt)
{
    Left_Backward();
    Right_Forward();
    Move(sp+300, sp+300);
    count = 0;
    while(1) {
        if (count > 90*cnt) {
            Move(0, 0);
            Clock_Delay1ms(300);
            direct += cnt;
            direct = direct % 8;
            count = 0;
            break;
        }
    }
}

void Turn_Right2(int sp, int cnt)
{
    Left_Forward();
    Right_Backward();
    Move(sp+300, sp+300);
    count = 0;
    while(1) {
        if (count > 90 * cnt) {
            Move(0, 0);
            Clock_Delay1ms(300);
            direct -= cnt;
            if (direct < 0)
                direct += 8;
            count = 0;
            break;
        }
    }
    Move(0,0);
}



int Search_Straight(int sp) {

    while(1) {
        Readysensor();
        if(!sensor4 && !sensor5)
        {
            Clock_Delay1ms(10);

            Move(0,0);

            return 1;

        } else if (sensor4 && !sensor5) {
            Left_Forward();
            Right_Forward();
            Move(sp,0);
        } else if (!sensor4 && sensor5) {
            Left_Forward();
            Right_Forward();
            Move(0, sp);
        } else if(sensor4 && sensor5){
            Left_Forward();
            Right_Forward();
            Move(sp, sp);
        } else {
            Left_Forward();
            Right_Forward();
            Move(sp, sp);
        }
        Clock_Delay1ms(10);
    }
}

typedef struct {
    int x;
    int y;
} Coordinate;

Coordinate direct_offset[8] = {
    {0, 1},   // 0: 북쪽
    {-1, 1},  // 1: 북서쪽
    {-1, 0},  // 2: 서쪽
    {-1, -1}, // 3: 남서쪽
    {0, -1},  // 4: 남쪽
    {1, -1},  // 5: 남동쪽
    {1, 0},   // 6: 동쪽
    {1, 1}    // 7: 북동쪽
};

typedef struct Node Node;

typedef struct Direction {
    int directNumber;       // 방향 넘버
    Node* node;             // 해당 방향의 노드
    int explored;           // 해당 방향의 탐색 여부 (0: 탐색 안함, 1: 탐색 완료)
} Direction;

typedef struct Node {
    int x;
    int y;
    int id;                     // 노드 ID
    int direction_count;        // 연결된 방향의 수
    Direction directions[8];  // 방향 배열
} Node;

Node* now_node;

#define MAX_NODES 50
Node node_pool[MAX_NODES];
int node_pool_index = 0;


Node* create_node() {
    if (node_pool_index >= MAX_NODES) {
        printf("Error: Maximum number of nodes reached\n");
        return NULL;
    }
    Node* node = &node_pool[node_pool_index];
    initialize_node(node);
    return node;
}


void initialize_node(Node* node) {
    node->x = 0;
    node->y = 0;
    node->id = node_pool_index++;
    node->direction_count = 0;
    int i;
    for (i = 0; i < 8; i++) {
        node->directions[i].directNumber = i;
        node->directions[i].node = NULL;
        node->directions[i].explored = 0;
    }
}

void add_direction(Node* src, int direction_index, Node* dest, int reverse_direction) {
    if (src->direction_count < 8) {
        src->directions[direction_index].node = dest;
        src->directions[direction_index].explored = 1;
        src->direction_count++;
    }

    if (dest->direction_count < 8) {
        dest->directions[reverse_direction].node = src;
        dest->directions[reverse_direction].explored = 1;
        dest->direction_count++;
    }
}

Node* find_or_create_node(int direction_index, Node* current) {
    if (current->directions[direction_index].explored == 0) {
        Node* new_node;
        if(!check_next(current->x + direct_offset[direction_index].x,
                current->y + direct_offset[direction_index].y))
        {
            new_node = create_node();
        }
        else{
            new_node = &node_pool[0];
        }
        int reverse_direction = (direction_index + 4) % 8;
        new_node->x = current->x + direct_offset[direction_index].x;
        new_node->y = current->y + direct_offset[direction_index].y;
        add_direction(current, direction_index, new_node, reverse_direction);

        return new_node;
    }
}

Node* Get_Dest(){
    int x = now_node->x,y = now_node->y,i;
    Node* cur;
    while(1)
    {
        x += direct_offset[direct].x;
        y += direct_offset[direct].y;
        for(i=0;i<node_pool_index;i++){
            cur = &node_pool[i];
            printf("curx: %d, cury : %d, x : %d, y : %d\n",cur->x,cur->y,x,y);
            if(x == cur->x && y == cur->y){
                return cur;
            }
        }
    }
}


void Go_Right(int sp) {
    int i;
    int flag = 1;
    for(i =0;i<8;i++){
        Readysensor();
        if ((sensor4 || sensor5) && !now_node->directions[direct].explored){

            flag = 0;
            break;
        }
        Turn_Right(sp);
    }
    if(flag == 1){
        phase = 2;
        now_node->directions[4].explored = 0;
        Turn_Left2(sp,4);
    }

    if(phase == 0)
    {
        //printf("now_node id : %d, x : %d, y: %d\n",now_node->id,now_node->x,now_node->y);
        Node* next_node = find_or_create_node(direct, now_node);
        now_node = next_node;
        Search_Straight(sp);
        printf("next_node id : %d, x : %d, y: %d\n",now_node->id,now_node->x,now_node->y);
    }
    if (phase == 1){
        Node* next_node = Get_Dest();
        printf("탈출\n");
        int reverse_direction = (direct + 4) % 8;
        add_direction(now_node, direct, next_node, reverse_direction);
        now_node = next_node;
        Search_Straight(sp);
        printf("next_node id : %d, x : %d, y: %d\n",now_node->id,now_node->x,now_node->y);
    }

}

int Find_Min_Direct() {
    int i = 1,j = 0, minus = -1;
    for(i = 0;i <= 4; i++) {
        int offset = i;
        int idx;
        for(j=0;j<2;j++){
            idx = direct;
            offset *= minus;
            idx += offset;
            idx = (idx + 8) % 8;
            if(now_node->directions[idx].explored){
                int target = idx - direct;
                if (target < 0)
                    target += 8;

                if(4 < target){
                    return 8 - target;
                } else {
                    return -1 * target;
                }
            }
        }
    }
    return 10;
}

void Go_Phase2(int sp) {
    Readysensor();
    int minDirect = Find_Min_Direct(), i = 0;
    if(minDirect > 0 && minDirect < 10){
        P2->OUT &= ~0x07;
        P1->OUT &= ~0x01;
        P2->OUT |= 0x1;
        P1->OUT |= 0x1;
        //for(i =0;i < minDirect;i++){
            Turn_Right2(sp,minDirect);
        //}
    }else if(minDirect < 0){
        P2->OUT &= ~0x07;
        P1->OUT &= ~0x01;
        P2->OUT |= 0x2;
        P1->OUT |= 0x2;
        minDirect *= (-1);
        //for(i =0;i < minDirect;i++){
            Turn_Left2(sp,minDirect);
        //}
    }else if(minDirect == 10){
        P2->OUT &= ~0x07;
        P1->OUT &= ~0x01;
        P2->OUT |= 0x4;
        P1->OUT |= 0x4;
        phase = 3;
    }
    int reverse_direction = (direct + 4) % 8;
    now_node->directions[direct].explored = 0;
    now_node->directions[direct].node->directions[reverse_direction].explored = 0;
    now_node = now_node->directions[direct].node;
    Search_Straight(sp);
}

int Is_Back_To_Start(Node* node) {
    return ((node->x == 0) && (node->y == 0));
}

int check_next(int x,int y) {
    return ((x == 0) && (y == 0));
}

void main(void) {
    LED();
    Clock_Init48MHz();
    Motor_Init();
    Sensor_Init();
    Timer_A3_Capture_Init();
    int sp = 2500;
    Node* start_node = create_node();
    start_node->x = 0;
    start_node->y = 0;
    now_node = start_node;
    now_node->directions[4].explored = 1;

    while(1) {
        Readysensor();
        if(sensor1 && sensor2 && sensor3 && sensor4 && sensor5 && sensor6 && sensor7 && sensor8)
        {
            Left_Forward();
            Right_Forward();
            Move(sp,sp);
            Clock_Delay1ms(200);
        } else if((!sensor1 && !sensor2 && sensor3 && sensor4 && sensor5 && sensor6 && sensor7 && sensor8)
                || (!sensor1 && sensor2 && sensor3 && sensor4 && sensor5 && sensor6 && sensor7 && sensor8))
        {
            Clock_Delay1ms(100);
            Turn_Left(sp);
            break;
        }
    }

    while(1) {
        Readysensor();
        Go_Right(sp);
        if(Is_Back_To_Start(now_node))
            {Move(0,0);
            phase = 1;
            break;}
    }
    while(1){
        if (phase == 2)
            break;
        Readysensor();
        Go_Right(sp);
    }
    while (1) {
        if(phase == 3) break;
        Go_Phase2(sp+3000);
    }
    printf("end\n");
}
