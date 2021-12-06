add_executable(client_tp src/client.cpp)
target_link_libraries(client_tp game player map server lib sfml-graphics sfml-window sfml-system sfml-audio ${Boost_LIBRARIES} -lpthread)
