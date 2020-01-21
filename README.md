# MKS65_Final_Project

## Ahmed Sultan, Brian Moses, Mohidul Abedin and Tejas Narayan — Systems Level Programming, pd 5

The main premise of the project is to create a music program that can play music and take inputs from clients as well as send relevant data. There are three distinct parts of this project: a music player that will be able to queue songs, a game that involves guessing a song playing from the server, and a basic library that users will be able to peruse.

## Features

  * Music player - Our program allows the user to play a song from a directory
  * Queue - Set up a queue system that will play songs one-by-one in a certain order
  * Game (think musical Kahoot!) - A server will play music and the client(s) will have to choose the correct response
  * GUI- All of this is on a window to make user interaction satisfying and easy

## How to Run

This project uitilizes networking, so you might need to ssh into the school computer atleast using 2 different ips.

    1. ssh <username>@149.89.150.101   &   <username>@149.89.150.102 (you can replace the last 3 
    digit number with anything from 101-135)
    2. git clone git@github.com:Mabedin00/MKS65_Final_Project.git
    3. cd MKS65_Final_Project
    4. make
    5. make run
    6. Select what you want.
    7. (Optional) This repo only provides 3 songs, if you want the DLC download follow these steps
        - Download the zip file from         
[here](https://drive.google.com/file/d/1gJHiYBtHHqpVVHr5IghHywaYKrwoG1ef/view?usp=sharing)  
        
        - Unzip and place the cotents into the MKS65_Final_Project/songs directory (THIS IS NOT PIRACY)


## Song player
 
    1. Simply select a song from file chooser prompt

## Song Queue

    1. Add songs to your queue from file chooser prompt
    2. You will see your queue on the left
    3. Click play queue to start

## Game
### Server side
    1. Select host
    2. Enter the amount of players and songs you want
    3. Share your IP with the clients
### Client Side
    1. Select client
    2. Enter the server IP and a username
    3. Wait for other players
    4. Try to guess the name of the song (your points is based on how much time was left)

## Bugs
    1. Entering more than 5 songs as the host will cause the buffers to overflow
        - This can be easily fixed by increasing BUFFER_SIZE in network.h, 
        but we found it unnecessary for testing purposes
    2. Duplicate answer choices
    3. There are un freed memory blocks, not sure where they went ¯\_(ツ)_/¯
