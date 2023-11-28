# URproj
 my undergraduate research project

## Compilation requirements:
 Docker

## Setup Instructions:
 open terminal, cd to urproj (root directory)

 ### Linux:
  run:
  `chmod +x build_docker_image.sh`
  `chmod +x run_docker_image.sh`
  `./build_docker_image.sh`

 ### Windows:
  run:
  `.\build_docker_image.bat`

## Compilation Instructions:
 open terminal, cd to urproj (root directory)

 ### Linux:
  run:
  `./run_docker_image.sh`

 ### Windows:
  run:
  `.\run_docker_image.bat`

 ### Finally:
  in the docker container:

  -for windows
  `make build_win`

  -for windows, with console
  `make build_win_console`

  -for linux
  `make build_linux`

  -to run VPLists test
  `make test_VPLists`

## Running Instructions
 once compilation has finished,
 go to /urproj/build

 ### Linux
  run
  `sudo apt install libglfw3`
  `chmod +x ./app`
  `./app`

 ### Windows
  (in file explorer) double click app.exe

  or

  (in powershell/cmd) run .\app.exe
