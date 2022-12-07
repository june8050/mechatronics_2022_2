# Mechatronics  
*Mechanical Engineering,Yonsei university*  
*Fall semester, 2022*  

  
이승준, 이상민

We learned 
  * **C-Language**
  * **Embeded System(with Raspberry PI)**
  * **Motor Control**
  * **Sensor**
  * **Electronic Circuit**

---
## Playing with LED
**Mole Game**  
*Source Code : HW2*  

Using Raspberry PI and LED, we play mole game!!

<p align = "center">
<img src = "https://user-images.githubusercontent.com/92682815/197863731-b70df64c-eff7-486b-b27f-7d2252508d52.gif">
</p>

---
## PID Control
**Motor Control**  
*Source Code : MidetermProject*  

<p align = "center">
<img src = "https://user-images.githubusercontent.com/92682815/197922939-996bd995-cd49-428a-98c9-ff20464ae844.png">
</p>

$$u = K_p e(t) + K_i \int^t_0 e(t)dt + K_d \frac{de}{dt}$$  
$K_p, K_i, K_d$ are called gain  
We optimize these by trials and errors

### Experiment  
*Optimization*  
Trial and Error  
At first, we find the proper P-Gain.  

<div>
<img src = "https://user-images.githubusercontent.com/92682815/197924284-87e98c8e-7530-4073-bff2-c1217f84aa5e.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/197924405-d77c4416-73cd-4be7-972f-13f958f3b9f8.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/197924504-2f296383-6c2f-4438-ae4a-6b2322dbfbef.png" width=30%>
</div>

### Test  
  * Pulse interval : 5sec 
<div>
<img src = "https://user-images.githubusercontent.com/92682815/197971098-1fb4cd03-58e7-4c4d-a585-fdf87f508877.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/197971112-238e8914-b4c1-4b62-8531-e7c391f47dc1.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/197971126-821da2ad-4f83-4112-beb7-d6ca57146499.png" width=30%>
</div>

### Result
condition
  * number of trials : 8  
    *  4 → 6 → 6 → -3 → 3 →-2 → 0 → 1
 <div>
 <img src = "https://user-images.githubusercontent.com/92682815/198462525-7418a56b-2020-47ae-b95c-0a083d2b4c1b.png" width = 45%/>
 <img src = "https://user-images.githubusercontent.com/92682815/198462544-51bab254-a88a-4ce7-b563-aa218e93a18f.png" width = 45%/>
</div>

**ITAE = 11.845**  
  where ITAE is  

$$ITAE = \int_0^{t_1} t|e_1|dt +\int_{t_1}^{t_2}(t-t_1)|e_2|dt \cdots \int_{t_{n-1}}^{t_n}(t-t_{n-1})|e_n|dt$$



---
## Point to Point Control in 2-Axis Feed System 

**Introduction**

두개의 다축 이송계를 통해 평면에서 우리가 원하는 위치까지 Work Table을 움직이는 task이다. 이송계를 통해 모터의 회전 운동을 선형운동으로 변환시키고, 작업물을 우리가 원하는 위치로 이송시킨다.

<p align = "center">
<img src = "https://user-images.githubusercontent.com/92682815/206187308-29d620fb-da36-4c97-a853-6754783d4b94.png">
</p>

그림과 같이 모터와 Screw를 Coupling을 해준후, 모터가 회전함에 따라 Screw가 같이 회전하도록 한다. 우리의 피치 간격은 2mm이고, 모터가 한 바퀴 회전할 때 Worktable은 2mm 이송된다.

<p align = "center">
<img src = "https://user-images.githubusercontent.com/92682815/206187364-1ec52b4a-55c1-4ba2-a1a7-663c372c321f.png">
</p>



하부 이송계가 x축, 상부 이송계가 y축이다. 우리는 라즈베리파이와 시뮬링크를 활용했고, 모터 2개를 회전시켜 2차원 평면에서 Worktable을 원하는 목표점 까지 이송시켰다. 모터는 PID를 활용해 제어했다.


우리는 다음과 같은 순서로 이번 task를 수행했다.

- **Problem 1 : PID 모터제어**  

    - 모터 2개를 PID를 통해 원하는 목표점(목표회전수) 만큼 회전 시켰다.  
    
- **Problem 2 : 직선경로 구현 1단계**  

    - 문제 1에서 목표 지점까지의 거리가 x축과 y축이 서로달라 직선 경로가 나오지 않았다. 즉, 더 작은 거리에 해당하는 축이 먼저 도착을 하고, 더 먼 거리에 대한 축은 이후에 계속 진행되었다.  
    
    - 직선 구현하기 위해 경로에 목표지점까지 한번에 가는 것이 아닌 여러개의 중간 목표(sub target)를 설정해 경로를 잘게 나눴다.  
    
- **Problem 3 : 직선경로의 구현 2단계**  

    - 속도가 0에서 목표하는 속도로 바로 바뀌어 가속도가 $\infty$가 되어 제어기가 잘 따라가지 못하고 경로에서 이탈하는 지점들이 보였다.  
    
    - 속도를 한번에 올리는 것이 아니라 가감속 구간을 정해 속도 변화를 천천히 하도록 만들었다.  
    
- **Problem 4 : 자유로운 직선경로의 제어**  

    - 한점에서 한점, 그리고 그점에서 다음 점으로 이동할 수 있도록 했다.  
    
- **Problem 5 : Path Planning**  

**Metric**

- 속도 오차 지표
    
    
    $$V_{avg} = \frac {\Delta t}{t_2 - t_1} \sum ^{t_2}_{t_1} V_{measurement}, D_{vel} = |\frac{V_{avg}-V_{command}}{V_{command}}|$$
    
    
- 경로 오차 지표
    
    
    $$Contour Error_{RMS} = \sqrt{\frac {\Delta t}{t_2 - t_1} \sum ^{t_2}_{t_1} |contour Error|^2}$$
    
    

Score = nomalization[ $D_{vel}$ ] * 30 + nomalization[ $Countour Error_{RMS}$ ] * 70

→ 우리는 주어진 속도 값으로 직선 경로를 따라가도록 프로그래밍을 해야한다.

---

### Trial and Error

task를 수행하기 위해 시도했던 방법, 문제 그리고 해결한 방식을 소개하겠다.

처음에 PID 모터 제어를 위해 블록을 아래와 같이 구성했다.
![Untitled 2](https://user-images.githubusercontent.com/92682815/206188695-5ed713a3-aef8-4520-8431-a8b6310bacd1.png)


Data Store Memory 블록을 이용해 Integral을, Unit Delay 블록을 이용해 에러 간 차이를 구했고 sampling time을 이용해 Derivative를 구현했었다. 1차 프로젝트와 같은 Gain 값을 그대로 사용했다. 

초기에 직선구간의 등분을 위해서 다음과 같은 알고리즘을 사용했다.

```matlab
%Pseudo Code
number reference_position %최종목표위치
number target %목표위치 까지 지속적으로 누적될 sub target
number current %엔코더로 측정한 현재 위치
number threshold %target과 current 값의 차가 threshold보다 작으면 다음 sub target으로 업데이트됨
number n %경로를 나눌 구간의 갯

if (reference_position >= target) %최종 목표위치에 도달하지 않았다면
	if (target-current > threshold) %sub target에 도달하지 못했다면
		do motor position control %모터 위치 제어를 수행
	else %sub target에 도달했다면
		target += refer/n %target을 다음 구간으로 업데이트
else
	end
```

아래는 위의 논리로 구간을 등분했을 때 얻은 구간 개수 n값에 따른 position에 대한 그래프이다. 

 <div>
 <img src = "https://user-images.githubusercontent.com/92682815/206187894-dd999fed-328f-41ca-8f59-3ddb6bc9f80e.png" width = 45%/>
 <img src = "https://user-images.githubusercontent.com/92682815/206187955-bbeab603-db10-436d-b50b-e7e829a2198d.png" width = 45%/>
</div>



처음엔 위와 같은 간단한 논리로 시뮬링크 블록만을 이용해 등분을 해서 모터를 돌리고자 했다. 그러나 모터가 reference position을 초과한 경우(refer - current < 0) 처리하지 못했고, 구간이 threshold보다 작으면 제어를 수행하지 않았다. 또한 가야할 목표지점이 충분히 멀지 않을 때 PID 에서 산출되는 gain이 모터를 돌릴만큼 크지 않은 값이었고 oscillation도 너무 심했다.

이후에는 매트랩 차원에서 timeseries 함수를 이용해 먼저 구간을 나눠 sub target들을 만들고 이를 이용해 sampling하도록 했다. 우리가 원하는 속도로 가도록 적절하게 등분을 하도록 프로그래밍을 했다. 

목표점이 주어지면 가야할 거리 d가 계산되고, 주어진 속도에대해 전체 걸려야 하는 시간을 구할 수 있다. 샘플링 타임을 0.001로 고정하면, 주어진 속도 v 에대해서 한번의 sampling에서 v * sampling time 만큼의 거리를 가도록 sub target의 time sereis를 만들었다. 그렇게 하면 총 구간의 개수N은 거리d/ (v*sampling time) 으로 구할 수 있다.

![Untitled 3](https://user-images.githubusercontent.com/92682815/206187713-71239c21-4324-4a7f-9967-affb243cc171.png)


PID 제어 블록을 시뮬링크의 integral과 dervative를 이용해 좀 더 간단하고 직관적이게 만들었다.

가감속 구간에서 가속도가 $\pm \infty$로 튀는 문제를 막기 위해 우리가 처음 생각한 방식은 직관적으로 모터에 공급되는 전원을 조절해보는 방식이었다. 그래서 PWM weight를 도입했다. 경로에 대해서 구간을 세개로 나눠 가속구간, 등속구간, 감속구간으로 설정한 후 각 구간에 대해 PWM weight를 다르게 줬다. 가속 구간에선 0→1로 커지는 weight를, 등속구간에서는 1의 weight를, 감속구간에선 1→ 0으로 작아지는 weight를 주었다.

이 PWM weight 그래프와 같은 형상의 속도 그래프가 나타나길 기대했고, 가감속 구간의 PWM weight step을 달리하여 실험했다.
![Untitled 4](https://user-images.githubusercontent.com/92682815/206188102-f7be0f35-df07-411c-9bd2-ae8285a7d9f1.png)

**Pipeline with PWM weight**
![Untitled 5](https://user-images.githubusercontent.com/92682815/206188264-55bb8ca8-3c48-45f4-ad5f-8434ab583303.png)


PWM weight range [0,1]

| step  | reached point | Error_max | Error_STD | Error_SteadyState | 목표점 까지 걸린 시간 | 속도의 분산 |
| --- | --- | --- | --- | --- | --- | --- |
| 0.01 | 63.91, 47.75 | 0.155556 | 0.051708 | 0.144 | - | - |
| 0.009 | 63.75, 47.96 | 0.188889 | 0.036197 | 0.12037037 |  8.14 |  14.86224 |
| 0.008 | 63.91, 48.03 | 0.183333 | 0.023537 | 0.072222222 | 8.16 | 14.54046 |
| 0.007 | 63.68, 47.71 | 0.159259 | 0.02243 | 0.035185185 | 8.11 | 14.89512 |
| 0.006 | 63.83, 47.90 | 0.17963 | 0.024429 | 0.018518519 | 8.128 | 14.90682 |
| 0.005 | 63.59, 47.51 | 0.148148148 | 0.025929961 | 0.148148148 | 8.09 | 14.49771 |
| 0.004 | 63.63, 47.51 | 0.211111 | 0.028332 | 0.17037037  | 8.13 | 14.98035 |
| 0.003 | 63.59, 47.63 | 0.2  | 0.025838 | 0.051851852 | 8.13 | 14.89104 |
| 0.002 | 63.31, 47.42 | 0.162963 | 0.022628 | 0.055555556 | 8.13 | 16.16758 |
| 0.001 | 62.91, 46.74 | 0.351851852 | 0.048457168 | 0.351851852 | 8.134 | 16.70466561 |
| Non PWM weight | 64.06, 48.06 | 0.34146464 |  0.052516085 | 0.09166202 |  8.285 | 15.82001 |




그러나 도착 지점에 근접했을 때 PID 제어를 바탕으로 나온 gain이 작은데 거기에 PWM도 작은 값이 곱해져 모터를 돌릴만큼 충분한 전원이 공급되지 않아 도착점에 도달하지 못했다. 그래서 PWM을 건드리는 것이 아니라 time series를 만들 때, sub targets의 간격을 조절하기로 했다.

---

### Main

속도는 시간 간격이 일정할 때, 위치차에 비례하므로 속도가 한번에  크게 증가하지 않게 하기 위해 점들(sub targets)간 간격을 조절해 가속과 감속을 구현했다. 

전체 구간을 3개로 나눠 가속구간, 등속구간, 감속구간을 정한다. 

여기서 앞 과 뒤 가속과 감속을 할 점들의 개수 n 을 임의로 정한다. 우리는 간단하게 가속과 감속 구간의 길이를 같게 했다.(총 2n)

![Untitled 6](https://user-images.githubusercontent.com/92682815/206189226-22173f18-86c4-44d8-b551-45d688dd26f7.png)


가속 구간의 간격의 공차를 d라 했을 때 $(n-1)d = v \cdot \Delta t$ 에서 d를 구하고, 이를 이용해 총 구간의 길이 $\frac {n(n-1)}{2}d$ 를 구한다. 우리는 가속과 감속구간의 길이를 같게 놓았으므로 등속구간은 총 구간길이에서 $\frac{n(n-1)}{2}d \cdot 2$  를 뺀 값이 된다. 등속구간 길이는 한 구간의 길이와 총 구간 개수 의 곱이므로

$$
N_{const} \cdot v_i \cdot \text{sampling time}= distance_i -\frac {n(n-1)}{2}d_i \cdot 2  \quad i = 1,2 
$$

 위의 식을 이용해 등속구간의 점개수$N_{const}$ 를 구할 수 있다. 여기서 i 값 1은 x, 2는 y에 대응된다.

```matlab
number sampling_time
number n %가속/감속구간 개수
number N %등속구간 개수
numer v,x,y %목표 속도, x좌표, y좌표
number d1 %가속구간 간격들의 공차
number d2 %감속구간 간격들의 공차

v_x, v_y = v의 x,y 성분

%가속/감속구간의 간격은 등차수열이고, 가속구간의 마지막 간격과 감속구간의 첫 간격이 등속구간의 간격과 일치해야한다.
%등속구간에서는 목표속도를 달성해야하므로 등속구간의 간격은 v*sampling_time
(n-1)*d1 = v_x*sampling_time;
(n-1)*d2 = v_y*sampling_time;
%따라서
d1 = v_x*sampling_time/(n-1);
d2 = v_y*sampling_time/(n-1);

position_x = [];
position_y = [];

for i = 1:n
	position_x, position_y에 가속구간 등차수열을 누적해서 넣어줌
for i = 1:N
	position_x, position_y에 각각 v_x*sampling_time, v_y*sampling_time을 N번 누적시켜줌
for i = 1:n
	position_x, position_y에 감속구간 등차수열을 누적해서 넣어줌
```

### Pipeline

![Untitled 7](https://user-images.githubusercontent.com/92682815/206189411-3b92a3f5-9b01-447a-80d3-2e63250da522.png)
우리 모델의 최종 pipeline이다. 각 문제 별로 매트랩 workspace에서 계산한 sub target들을 x_series, y_series를 통해 입력해줬다. 

---
### Problem 1



<div>
<img src = "https://user-images.githubusercontent.com/92682815/206189645-83568c92-50e9-4d35-840d-bd13310336a1.png" width=40%>
<img src = "https://user-images.githubusercontent.com/92682815/206189652-7d64608a-e890-4f75-9856-be9858ad8328.png" width=40%>
</div>

왼쪽의 xy plot을 보면 직선 경로가 되지 않음 볼 수 있다. 이는 각 목표지점에대한 거리가 다른데, 속도에 대한 정보 없이 x축과 y축이 서로 독립적으로 제어를 수행했기 때문이다.

---

### Problem 2

**Sampling Code**

```matlab
function [x_series, y_series] = route_divider_2(v,x,y)

    d = sqrt(x^2+y^2);
    N = d/(0.001*v);

    x_series = timeseries(0:x/N:x,0:0.001:d/v);
    y_series = timeseries(0:y/N:y,0:0.001:d/v);
end
```

<div>
<img src = "https://user-images.githubusercontent.com/92682815/206189734-46969427-a0ab-4589-a633-e2a240909143.png" width=40%>
<img src = "https://user-images.githubusercontent.com/92682815/206190228-07df3120-1943-48be-9520-248c5f61e8bf.png" width=40%>
</div>


속도 그래프는 moving average를 이용해 plot을 했다.

초반( t= 0 근처)에 가속도가 $\infty$로 튀어 그래프가 직선에서 벗어난 모습을 볼 수있다.


---

### Problem 3

**Sampling Code**

```matlab
sampling_time = 0.001;
n =1000; %가속 감속 구간 개수 -> 총 가감속 구간 1000*2 = 2000

%목표 속도와 위치 좌표
v=5; % velocity
x = 64; %reference_x
y = 48; %reference_y

theta = atan2(y,x) % y/x 각도

v_x = v*cos(theta) % v의 x성분
v_y = v*sin(theta) % v의 y성분

d1 = v_x*sampling_time/(n-1) % x 간격의 공차 with 마지막 간격 (n-1)d = 등속구간의 간격 v_x * sampling time
d2 = v_y*sampling_time/(n-1) % y 간격의 공차

integral_x=0; % 구간 간격간 등차수열을 이루도록 점들을 구하기 위한 변수
integral_y=0;

accel_x=[]; % 가속 구간 점들
accel_y=[]; 

for i = 1:n
    accel_x(i)=integral_x;
    accel_y(i)=integral_y;
    integral_x = integral_x + i*d1;
    integral_y = integral_y + i*d2;
end

position_x = accel_x;
position_y = accel_y;

%나눠떨어지지 않을 경우를 대비해서 반올림
N = round((x-n*(n-1)*d1)/(v_x*sampling_time))

for i = 1:N
    position_x(n+i)=position_x(n+i-1)+(v_x*sampling_time);
    position_y(n+i)=position_y(n+i-1)+(v_y*sampling_time);
end

for i = 1:n
    position_x(n+N+i)=position_x(n+N+i-1)+(n-i)*d1;
    position_y(n+N+i)=position_y(n+N+i-1)+(n-i)*d2;
end

time = sampling_time:sampling_time:(N+2*n)*sampling_time;

x_series = timeseries(position_x, time);
y_series = timeseries(position_y, time);
```


<div>
<img src = "https://user-images.githubusercontent.com/92682815/206190379-e6a75fa7-0e52-41ad-94f8-a85bdbedb9bf.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/206190384-2a2e98fb-0a33-45d8-86b1-699f1753bce3.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/206190392-bc226ebd-26e8-4fb5-8551-599905eb921e.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/206190398-46e8d306-6627-4b55-8a28-7556e20eefa0.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/206190403-181a206c-7470-4f4d-98b7-db080e5d8593.png" width=30%>
<img src = "https://user-images.githubusercontent.com/92682815/206190410-d1e6486a-4f4a-44d9-9c1c-11ce12957a44.png" width=30%>
</div>



가속과 감속할 구간의 점들의 개수 n 을 통해 우리가 원하는 속도 커브를 만들어 냈고, n값에 따른 여러가지 속도 커브에 대해 실험을 했다.

n > 1000 이상에서 도달 시간만 늦어지고 error가 주는 모습은 보이지 않았기 때문에, n = 1000일 때를 최적 속도 커브라고 판단했다.


---

### Problem 4

```matlab
sampling_time = 0.001;
n =1000;

%목표 위치와 속도
v=1;
x1 = 64;
y1 = 48;
x2 = 30;
y2 = -30;

%상대적인 위치 (reference)
x2 = x2 - x1;
y2 = y2 - y1;

theta1 = atan2(y1,x1);
theta2 = atan2(y2,x2);

v1_x = v*cos(theta1)
v1_y = v*sin(theta1)

v2_x = v*cos(theta2)
v2_y = v*sin(theta2)

%가속 구간과 감속 구간 간격의 공차
d1_x = v1_x*sampling_time/(n-1)
d1_y = v1_y*sampling_time/(n-1)

d2_x = v2_x*sampling_time/(n-1)
d2_y = v2_y*sampling_time/(n-1)

integral_x=0;
integral_y=0;

accel_x=[];
accel_y=[];
%Add first accel section
for i = 1:n
    accel_x(i)=integral_x;
    accel_y(i)=integral_y;
    integral_x = integral_x + i*d1_x;
    integral_y = integral_y + i*d1_y;
end

position_x = accel_x;
position_y = accel_y;

%N1 and N1_y has to be equal
%나눠떨어지지 않을경우를 대비해 반올림
N1 = round((x1-n*(n-1)*d1_x)/(v1_x*sampling_time))
N1_y = round((y1-n*(n-1)*d1_y)/(v1_y*sampling_time))

%Add first constant vel section
for i = 1:N1
    position_x(n+i)=position_x(n+i-1)+(v1_x*sampling_time);
    position_y(n+i)=position_y(n+i-1)+(v1_y*sampling_time);
end
%Add first decel section
for i = 1:n
    position_x(n+N1+i)=position_x(n+N1+i-1)+(n-i)*d1_x;
    position_y(n+N1+i)=position_y(n+N1+i-1)+(n-i)*d1_y;
end

%N2 and N2_y has to be equal
%나눠떨어지지 않을경우를 대비해 반올림
N2 = round((x2-n*(n-1)*d2_x)/(v2_x*sampling_time))
N2_y = round((y2-n*(n-1)*d2_y)/(v2_y*sampling_time))

%Add second accel section
for i = 1:n
    position_x(N1+2*n+i)=position_x(N1+2*n+i-1)+i*d2_x;
    position_y(N1+2*n+i)=position_y(N1+2*n+i-1)+i*d2_y;
end

%Add second constant vel section
for i = 1:N2
    position_x(N1+3*n+i)=position_x(N1+3*n+i-1)+(v2_x*sampling_time);
    position_y(N1+3*n+i)=position_y(N1+3*n+i-1)+(v2_y*sampling_time);
end

%Add second decel section
for i = 1:n
    position_x(N1+N2+3*n+i)=position_x(N1+N2+3*n+i-1)+(n-i)*d2_x;
    position_y(N1+N2+3*n+i)=position_y(N1+N2+3*n+i-1)+(n-i)*d2_y;
end

time = sampling_time:sampling_time:(N1+N2+4*n)*sampling_time;

x_series = timeseries(position_x, time);
y_series = timeseries(position_y, time);
```

$(x_1,y_1)$ = (64,48)  → $(x_2,y_2)$ = (30,-30) 으로 설정한 후 모터를 돌렸다.

속도 커브 n = 1000

![prob4](https://user-images.githubusercontent.com/92682815/206190606-b79aa0aa-3998-4004-a068-d9d6348d0cb4.png)
![v_5](https://user-images.githubusercontent.com/92682815/206190682-e6ab21fa-f23a-44a4-b449-b3671b5d3b7c.png)


---

### Problem 5

앞선 문제들에서 가속/감속구간을 직선거리 기준으로 나누었다면, 본 문제에서는 각도를 기준으로 가속/감속 구간을 나눠봤다.

또한 각도가 90도, 180도, 270도를 지날때마다 x 또는 y 모터의 회전방향이 바뀌므로 가속도가 $\infty$가 되기 때문에, 해당 구간들에도 가속/감속 구간을 추가해줬다. 전체적인 도식도는 다음과 같다.

![Untitled 8](https://user-images.githubusercontent.com/92682815/206190888-035c7680-8267-451f-bccb-ec957517f046.png)


해당 논리를 바탕으로 다음 코드를 이용해 x좌표와 y좌표 각각의 timeseries를 만들었다.

```matlab
%임의의 목표 반지름과 목표 속도
r = 40;
v = 5;

%가속/감속 구간의 공차
d = 0.00001;

sampling_time = 0.001;

%등속구간의 간격
theta_c = v*0.001*360/(2*pi*r)

%가속/감속구간의 각도간격
accel = 0:d:theta_c
decel = theta_c:-d:0

%가속/감속 구간동안 이동한 각도
l = length(accel)
accel_angle = sum(accel)

%등속구간 개수 = 등속구간동안 이동해야 하는 각도 / 등속구간 간격
N = round((90-accel_angle*2)/theta_c)

%각도간격들을 0부터 누적시켜줌
angle = [d];
%first accel
for i = 2:l
    angle(i)=angle(i-1)+i*d;
end
%first constant
for i=1:N
    angle(l+i)=angle(l+i-1)+theta_c;
end
%first decel
for i= 1:l
    angle(l+N+i)=angle(l+N+i-1)+(l-i+1)*d;
end
%second accel
for i = 1:l
    angle(N+2*l+i) = angle(N+2*l+i-1)+i*d;
end
%second constant
for i=1:N
    angle(N+3*l+i) = angle(N+3*l+i-1)+theta_c;
end
%second decel
for i = 1:l
    angle(2*N+3*l+i) = angle(2*N+3*l+i-1)+(l-i+1)*d;
end
%third accel
for i = 1:l
    angle(2*N+4*l+i) = angle(2*N+4*l+i-1)+i*d;
end
%third constant
for i=1:N
    angle(2*N+5*l+i) = angle(2*N+5*l+i-1)+theta_c;
end
%third decel
for i = 1:l
    angle(3*N+5*l+i) = angle(3*N+5*l+i-1)+(l-i+1)*d;
end
%fourth accel
for i = 1:l
    angle(3*N+6*l+i) = angle(3*N+6*l+i-1)+i*d;
end
%fourth constant
for i=1:N
    angle(3*N+7*l+i) = angle(3*N+7*l+i-1)+theta_c;
end
%fourth decel
for i = 1:l
    angle(4*N+7*l+i) = angle(4*N+7*l+i-1)+(l-i+1)*d;
end

x_position=[];
y_position=[];

%각도정보를 바탕으로 x와 y가 이동해야하는 위치정보를 계산
for i = 1:length(angle)
    x_position(i) = r*cosd(angle(i))-r;
    y_position(i) = r*sind(angle(i));
end

time = sampling_time:sampling_time:sampling_time*length(angle);

x_series = timeseries(x_position,time);
y_series = timeseries(y_position,time);
```

이렇게 만들어진 x와 y의 timeseries를 바탕으로 모터를 구동한 결과는 다음과 같다.
![prob5](https://user-images.githubusercontent.com/92682815/206190941-b44baca0-0dad-470c-a36e-d2d4e8bde7b3.png)


