HTTP server from scratch


Components:
1. TCP socket, binding to ports, listening to connections(base web server)
2. Request protocol parser - http request lines and headers
3. API routing 
4. Handing headers
5. Multiple connections
6. Serve static files


Sources: 
1. https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7
2. https://developer.mozilla.org/en-US/docs/Web/HTTP


list all wsl distros -> wsl -l -v
terminate wsl -> wsl --terminate <distro>
set default wsl distro -> wsl --set-default <distro>
install a distro -> wsl --install -d <distro>
install packages in ubuntu -> sudo apt install build-essential cmake git -y

to build the project
- cd build
- cmake ..
- make
- ./http_server