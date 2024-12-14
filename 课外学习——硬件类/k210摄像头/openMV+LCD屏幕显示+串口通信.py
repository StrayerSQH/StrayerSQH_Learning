import sensor, image, time, lcd
from machine import UART
from fpioa_manager import fm

def ChessPosition(Chess_x, Chess_y):
    One_Range_x = (0, 50)
    One_Range_y = (0, 50)
    Width = 40

    if One_Range_x[0] <= Chess_x <= One_Range_x[1] and One_Range_y[0] <= Chess_y <= One_Range_y[1]:
        return 1
    elif One_Range_x[0] + Width <= Chess_x <= One_Range_x[1]  + Width and One_Range_y[0] <= Chess_y <= One_Range_y[1]:
        return 2
    elif One_Range_x[0] + Width * 2 <= Chess_x <= One_Range_x[1] + Width * 2 and One_Range_y[0] <= Chess_y <= One_Range_y[1]:
        return 3
    elif One_Range_x[0] <= Chess_x <= One_Range_x[1] and One_Range_y[0] + Width <= Chess_y <= One_Range_y[1] + Width:
        return 4
    elif One_Range_x[0] + Width <= Chess_x <= One_Range_x[1] + Width and One_Range_y[0] + Width <= Chess_y <= One_Range_y[1] + Width:
        return 5
    elif One_Range_x[0] + Width * 2 <= Chess_x <= One_Range_x[1]  + Width * 2 and One_Range_y[0] + Width <= Chess_y <= One_Range_y[1] + Width:
        return 6
    elif One_Range_x[0] <= Chess_x <= One_Range_x[1] and One_Range_y[0]  + Width * 2 <= Chess_y <= One_Range_y[1] + Width * 2:
        return 7
    elif One_Range_x[0] + Width <= Chess_x <= One_Range_x[1] + Width and One_Range_y[0] + Width * 2 <= Chess_y <= One_Range_y[1] + Width * 2:
        return 8
    elif One_Range_x[0] + Width * 2 <= Chess_x <= One_Range_x[1] + Width * 2 and One_Range_y[0] + Width * 2 <= Chess_y <= One_Range_y[1] + Width * 2:
        return 9
    else:
        return 0

# 摄像头模块初始化
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)  # 使用 GRAYSCALE 灰度图像处理速度会更快
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=1000)
clock = time.clock()

# LCD 初始化
lcd.init()

#串口通信初始化
fm.register(10, fm.fpioa.UART1_TX, force=True)
uart = UART(UART.UART1, 115200, 8, 0, 0, timeout=1000, read_buf_len=256)

#黑色棋子和白色棋子位置
BlackChessPosition = []
WhiteChessPosition = []

black_positions_str = "B"
white_positions_str = "W"

while True:
    clock.tick()

    img = sensor.snapshot().lens_corr(0.1)

    black_rectangles = 0
    white_circles = 0
    black_circles = 0

    # 白色圆圈识别
    for c in img.find_circles(threshold=2000, x_margin=10, y_margin=10, r_margin=10, r_min=10, r_max=15, r_step=2):
        # 获取圆圈中心点颜色
        center_color = img.get_pixel(c.x(), c.y())
        if center_color > 150:  # 增加对白色的灰度值阈值
            white_circles += 1
            img.draw_circle(c.x(), c.y(), c.r(), color=(0, 0, 255))
            img.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 0))
            m = ChessPosition(c.x(), c.y())
            if m not in WhiteChessPosition:  # 检查m是否已存在于列表中
                WhiteChessPosition.append(m)  # 将m添加到列表中
                WhiteChessPosition.sort()
                white_positions_str += str(m)

        elif center_color < 50:  # 如果中心点颜色较暗，则认为是黑色圆圈
            black_circles += 1
            img.draw_circle(c.x(), c.y(), c.r(), color=(0, 0, 255))
            img.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 0))
            n = ChessPosition(c.x(), c.y())
            if n not in BlackChessPosition:  # 检查m是否已存在于列表中
                BlackChessPosition.append(n)  # 将m添加到列表中
                BlackChessPosition.sort()
                black_positions_str += str(n)

    # 在LCD上显示图像
    lcd.display(img)

    # 在LCD上显示黑色边框矩形和圆圈的个数信息
    lcd.draw_string(0, 10, "White circles: %d" % white_circles, lcd.WHITE)
    lcd.draw_string(0, 20, "Black circles: %d" % black_circles, lcd.WHITE)
    lcd.draw_string(0, 30, "Positions: %s%s" % (black_positions_str, white_positions_str), lcd.WHITE)

    print(black_positions_str + white_positions_str)
    #将棋子坐标进行发送
    uart.write(black_positions_str + white_positions_str)
