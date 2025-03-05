0 install docker desktop

1 docker build . -t ubuntu-dev

2.0 docker run ubuntu-dev - exits?

2.1 docker exec -it ubuntu-dev /bin/bash ?

3 docker run -dit ubuntu-dev
  ... (ctrl+d)

4 docker start -ai theContainer

5 docker ps -a

6 docker rename oldName newname

7 install vscode

8 add dev-container extension

9 should be able to start any container from system including "theContainer"

10 vsc make its own image when builds container...

11 ctrl+shift+p: Run Task then choose task from tasks.json

12 install extensions c++ and cmake , create CMakeLists.txt inside sc folder, it will automatically link build folder and build button will be created.

ctrl+shift+b
