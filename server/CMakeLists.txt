cmake_minimum_required(VERSION 3.22)
# 1.12 server stuff for ease of use

set(SERVER_DIR "${CMAKE_SOURCE_DIR}/server")
set(SERVER_JAR "${SERVER_DIR}/server.jar")
set(SERVER_URL "https://piston-data.mojang.com/v1/objects/8494e844e911ea0d63878f64da9dcc21f53a3463/server.jar")

# Custom target: download-server
add_custom_target(download-server
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SERVER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Downloading Minecraft server JAR..."
    COMMAND curl -L "${SERVER_URL}" -o "${SERVER_JAR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Download complete: ${SERVER_JAR}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Downloading Minecraft server JAR"
)

# Custom target: start-server
add_custom_target(start-server
        COMMAND ${CMAKE_COMMAND} -E echo "Starting Minecraft server..."
        COMMAND java -Xmx1024M -Xms1024M -jar server.jar
        WORKING_DIRECTORY "${SERVER_DIR}"
        COMMENT "Starting Minecraft server"
        USES_TERMINAL
)
