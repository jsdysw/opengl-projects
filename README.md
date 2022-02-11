# opengl-projects
swimming cube man
*ID : 20174089

*NAME : Seokwon Yoon

*How to Compile 
- OS type : Windows
- Used Language : ISO C++14 Standard
- Compiler type/version : Microsoft (R) C/C++ Optimizing Compiler Version 19.28.29913 for x86
- run "VirtualLego.sln" file with Visual Studio 2017 -> click Debug tab -> click Start Without Debugging

*How to Execute
- Option 1. run VirtualLego.sln file with Visual Studio 2017 -> click Debug tab -> click Start Without Debugging
- Option 2. execute "VirtualLego.exe" at root folder

* Summary of Code
- const float spherePos[][] : it contains the coordinate of yellow balls which you have to hit and erase
- CSphere::hitBy(CSphere& ball) : check if two balls collide, and calculate the new reflected movement direction and speed.
- CSphere::ballUpdate() : update ball's state(Position, power, ..etc) at every frame(timeDiff). Additionally, make the ball safe not to go outside of the board
- CWall::hitBy(CSphere& ball) : check if the ball hits walls, and calculate the new reflected movement direction and speed.
- Setup() : This function is called once and setup all the initial states of the Map
- Display(float timeDelta) : This function is called every frame. Detect collide events and update all the states of the balls before drawing
- CALLBACK d3d::WndProc(..) : WM_MOUSEMOVE(move white ball), WM_KEYDWON, VK_SPACE(start game)
