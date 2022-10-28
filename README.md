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

