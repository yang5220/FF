import pygame, random
from pygame.locals import*

#2
pygame.init()
score=0
width, height = 640, 480
screen=pygame.display.set_mode((width, height))
keys=[False, False]
playerpos = [500, 400]
eggtimer = 100
eggtimer1 = 0
xxtimer=150
xxtimer1=0
eggguys=[[300, 30]]
xxguys=[[300, 30]]
pygame.mixer.init()
#3
player=pygame.image.load("images/tuanzi.png")
grass=pygame.image.load("images/grass.png")
eggguyimg1=pygame.image.load("images/egg.png")
xx=pygame.image.load("images/X.png")
win=pygame.image.load("images/youwin.png")
defeat=pygame.image.load("images/gameover.png")
hit=pygame.mixer.Sound("music/shoot.wav")
bomb=pygame.mixer.Sound("music/X.wav")
clap=pygame.mixer.Sound("music/clap.wav")
pygame.mixer.music.load('music/long.wav')
pygame.mixer.music.play(-1, 0.0)
pygame.mixer.music.set_volume(0.25)
hit.set_volume(0.05)
bomb.set_volume(0.05)
clap.set_volume(0.05)
eggguyimg=eggguyimg1
#4
running=1
exitcode=0
while running:
    eggtimer-=1
    xxtimer-=1

    #5
    screen.fill(0)
    #6
    for x in range(int(width/grass.get_width()+1)):
        for y in range(int(height/grass.get_height()+1)):
            screen.blit(grass, (x*100, y*100))
    screen.blit(player, playerpos)
    #6.3，生成鸡蛋并判断是否与团子位置重合
    if eggtimer==0:
        eggguys.append([random.randint(50,430),30])
        eggtimer=100-(eggtimer1*2)
        if eggtimer1>=35:
            eggtimer1=35
        else:
            eggtimer1+=5
    index=0
    for eggguy in eggguys:
        if eggguy[1]>=height:
            eggguys.pop(index)
        eggguy[1]+=5
        eggrect = pygame.Rect(eggguyimg.get_rect())
        eggrect.top = eggguy[1]
        eggrect.left = eggguy[0]
        playrect=pygame.Rect(player.get_rect())
        playrect.top=playerpos[1]
        playrect.left=playerpos[0]
        if playrect.colliderect(eggrect):
            score+=1
            eggguys.pop(index)
            hit.play()
        index+=1
    for eggguy in eggguys:
        screen.blit(eggguyimg, eggguy)


#随机生成手榴弹，并判断是否与团子位置重合
    if xxtimer==0:
        xxguys.append([random.randint(50,430),30])
        xxtimer=300-(xxtimer1*2)
        if xxtimer1>=55:
            xxtimer1=55
        else:
            xxtimer1+=5
    index1=0
    for xxguy in xxguys:
        if xxguy[1]>=height:
            xxguys.pop(index1)
        xxguy[1]+=2
        xxrect = pygame.Rect(xx.get_rect())
        xxrect.top = xxguy[1]
        xxrect.left = xxguy[0]
        playrect=pygame.Rect(player.get_rect())
        playrect.top=playerpos[1]
        playrect.left=playerpos[0]
        if playrect.colliderect(xxrect):
            running=0
            xxguys.pop(index1)
            bomb.play()
            pygame.mixer.music.stop()
        index1+=1
    for xxguy in xxguys:
        screen.blit(xx, xxguy)
    #7
    pygame.display.flip()

    #8，通过按键信息判断团子的运动状况
    for event in pygame.event.get():
        if event.type==pygame.QUIT:
            pygame.quit()
            exit(0)
        if event.type==pygame.KEYDOWN:
            if event.key==K_LEFT:
                keys[0]=True
            elif event.key==K_RIGHT:
                keys[1]=True
        if event.type==pygame.KEYUP:
            if event.key==K_LEFT:
                keys[0]=False
            elif event.key==K_RIGHT:
                keys[1]=False
    if keys[0]:
        playerpos[0]-=8
    elif keys[1]:
        playerpos[0]+=8



    if pygame.time.get_ticks()>=90000:
        running=0
        exitcode=1
    if exitcode==1:
        pygame.font.init()
        font=pygame.font.SysFont("Times NewRoman", 24)
        text=font.render("Score:"+str(score), True, (0, 255, 0))
        textRect=text.get_rect()
        textRect.centerx=screen.get_rect().centerx
        textRect.centery=screen.get_rect().centery+18
        if score>=20:
            clap.play()
            screen.blit(win,(0,0))
            pygame.mixer.music.stop()
        else:
            screen.fill(255)
            screen.blit(defeat,(0,0))
            pygame.mixer.music.stop()
        screen.blit(text, textRect)


    else:
        pygame.font.init()
        font = pygame.font.SysFont("Times NewRoman", 24)
        text = font.render("Score:" + str(score), True, (0, 255, 0))
        textRect = text.get_rect()
        textRect.centerx = screen.get_rect().centerx
        textRect.centery = screen.get_rect().centery + 18
        screen.fill(255)
        screen.blit(defeat, (0, 0))
        screen.blit(text, textRect)


while 1:
    for event in pygame.event.get():
        if event.type==pygame.QUIT:
            pygame.quit()
            exit(0)
        pygame.display.flip()

