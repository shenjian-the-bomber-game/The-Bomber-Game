

# Simple Chat Room Package Design  

## Frame type (all)

|  frame type  | descriptor |
| :-----------------: | :--------: |
| info | 0x00 |
| info_response | 0x01 |
| passwd | 0x02 |
| passwd_response | 0x03 |
| refuse | 0x04 |
| UserName | 0x06 |
|   OnlineUser    |    0X07    |
|     SyncEnd     |    0x08    |
|    SendInvit    |    0x09    |
| RecvInvit | 0x0A |
| InvitResponse | 0x0B |
| Board | 0x0C |
| SingleCoord | 0x0D |
| DoubleCoord | 0x0E |
| GameOver | 0x0F |



------------------------

## Log In

### Frame types(Log In)

|   frame type    | descriptor |
| :-------------: | :--------: |
|      info       |    0x00    |
|  info_response  |    0x01    |
|     passwd      |    0x02    |
| passwd_response |    0x03    |
|     refuse      |    0x04    |

------------------------

#### info `0x00`

|  0   |         1 , 2         |     3 ... 31      |
| :--: | :-------------------: | :---------------: |
| 0x00 | info_length (2 bytes) | data（user_name） |

#### info_response `0x01`

|  0   |  1, 2  |      3       |
| :--: | :----: | :----------: |
| 0x01 | 0x0001 | ResponseType |

 > **ResponseType:**
 >
 > > >|         0x0         | 0x1  |     0x4     |
 > > >| :-----------------: | :--: | :---------: |
 > > >| user does not exist |  OK  | ErrorOccurs |

#### passwd `0x02`

|  0   |          1 , 2          |        3 ... 31         |
| :--: | :---------------------: | :---------------------: |
| 0x02 | passwd_length (2 bytes) | password (28 bytes max) |

#### passwd_response `0x03`

|  0   |  1, 2  |      3       |
| :--: | :----: | :----------: |
| 0x03 | 0x0001 | ResponseType |

> **ResponseType:**

> | 0x1  |  0x3  |     0x4     |
> | :--: | :---: | :---------: |
> |  OK  | wrong | ErrorOccurs |

#### refuse  `0x04`

|  0   |  1, 2  |      3       |
| :--: | :----: | :----------: |
| 0x04 | 0x0001 | responseType |

>**ResponseType:**
>
>|   **0x4**   |     **0x5**     |
>| :---------: | :-------------: |
>| ErrorOccurs | AlreadyLoggedIn |





-----------

## Sync

### Frame types

| frame type  | descriptor |
| :---------: | :--------: |
|  UserName   |    0x06    |
| OnlineUser  |    0X07    |
|   SyncEnd   |    0x08    |
| OfflineUser |    0x10    |



#### **UserName**  `0x06`

|  0   |            1, 2            |         3 ... 31         |
| :--: | :------------------------: | :----------------------: |
| 0x06 | user_name length (2 bytes) | user_name (host to user) |

#### **OnlineUser** `0x07`

|  0   |            1,2            |        3... 31        |
| :--: | :-----------------------: | :-------------------: |
| 0x07 | user_name length(2 bytes) | user_name (broadcast) |

#### **OfflineUser** `0x10`

|  0   |            1,2            |        3... 31        |
| :--: | :-----------------------: | :-------------------: |
| 0x10 | user_name length(2 bytes) | user_name (broadcast) |

#### SyncEnd   `0x08`

|  0   | 1， 2  |  3   |
| :--: | :----: | :--: |
| 0x08 | 0x0000 |  0   |



------------------------

## Game

**Frame types**

|  frame type   | descriptor |
| :-----------: | :--------: |
|   SendInvit   |    0x09    |
|   RecvInvit   |    0x0A    |
| InvitResponse |    0x0B    |
|     Board     |    0x0C    |
|  SingleCoord  |    0x0D    |
|  DoubleCoord  |    0x0E    |
|   GameOver    |    0x0F    |

#### **SendInvit** **0x09**

|  0   |            1,2             |         3... 31          |
| :--: | :------------------------: | :----------------------: |
| 0x09 | user_name length (2 bytes) | user_name (host to user) |

#### **RecvInvit** **0x0A**

|  0   |            1,2             |         3... 31          |
| :--: | :------------------------: | :----------------------: |
| 0x0A | user_name length (2 bytes) | user_name (host to user) |

#### **InvitResponse** **0x0B**

|  0   | 1,2  |      3       |
| :--: | :--: | :----------: |
| 0x0A |  1   | ResponseType |

  >**ResponseType:**
  >
  >|      0       |  1   |      2      |  6   |
  >| :----------: | :--: | :---------: | :--: |
  >| UserNotExist |  OK  | RefuseInvit | Busy |

 

#### **Board 0x0C**

|  0   | 1,2  |                           3... 14                            |
| :--: | :--: | :----------------------------------------------------------: |
| 0x0C |  12  | $ h_1(x,y), t_1(x,y), h_2(x,y), t_2(x,y), h_3(x,y), t_3(x,y) $ |



#### **SingleCoord 0x0D**

|  0   | 1,2  | 3,4  |
| :--: | :--: | :--: |
| 0x0D |  2   | x,y  |



####  **DoubleCoord 0x0E**

|  0   | 1,2  |          3,4          |
| :--: | :--: | :-------------------: |
| 0x0E |  4   | $ h_x, h_y, t_x,t_y $ |



#### **GameOver 0x0F**

|  0   | 1,2  |  3   |
| :--: | :--: | :--: |
| 0x0F |  0   | null |

>If you win, your client will send frame GameOver to the server;
>
>If you lose, your client will recv frame GameOver from the server.

---------------------

## Game board design

#### Game Board values


$$
n\ 飞机编号 \ \ n=1,2,3 \\
t\ 飞机部位 \ \  t=1\ 表示机身, t=2\ 表示机头，t=3\ 表示机尾 \\ 
g\ 对方是否猜出 \ \ g=0\ 未猜出，g=3\ 已猜出
\ \\
board[i][j] = 
\begin{align}
n*10+t+g\\
\end{align}
$$





#### Game Board Color Design

| 数值 |            含义            |                             颜色                             |
| :--: | :------------------------: | :----------------------------------------------------------: |
|  0   |         miss 猜错          | #F5DEB3 ![1557549711128](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557549711128.png) |
|  1   |   planeBody 己方猜中机身   | #CDC9C9 ![1557551136514](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557551136514.png) |
|  2   |   planeHead 己方猜中机头   | #8B7D7B ![1557550150989](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557550150989.png) |
|  3   |   planeTail 己方猜中机尾   | #8B7D7B ![1557551211173](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557551211173.png) |
|  4   |    hitBody 对方猜中机身    | #008B8B ![1557550061058](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557550061058.png) |
|  5   |    hitHead 对方猜中机头    | #8B0000 ![1557549462712](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557549462712.png) |
|  6   |    hitTail 对方猜中机尾    | #8B008B ![1557549494102](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557549494102.png) |
|  7   | doubleCoordHead 双坐标机头 | #EE6A50 ![1557549547836](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557549547836.png) |
|  8   |       blink 闪烁提示       | #000000 ![1557549585079](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557549585079.png) |
|  9   |       notKnown 未知        | #E0EEE0 ![1557550328978](C:\Users\UCRRR\AppData\Roaming\Typora\typora-user-images\1557550328978.png) |



----------

