# MKS65_Final_Project

## Ahmed Sultan, Brian Moses, Mohidul Abedin and Tejas Narayan — Systems Level Programming, pd 5

The main premise of the project is to create a music program that can play music and take inputs from clients as well as send relevant data. There are three distinct parts of this project: a music player that will be able to queue songs, a game that involves guessing a song playing from the server, and a basic library that users will be able to peruse.

## Features

  * Music player - The program will be able to use a directory of songs and play it through execvp and certain command line commands.
      - We are trying to figure out the best way to play these songs. (through the usage of a CLI/something else)
  * Requests/queue/playlist - The program will set a connection between multiple clients and take requests of songs. We will most likely use a combination of semaphores and pipes to create a music queue.
      - Additionally, we can maybe download unavailable songs. (not sure though)
  * Game (think musical Kahoot!) - Using the network system, the clients can also participate in a quiz game where they guess the name of the song. The server will take inputs and see who got the answer first. Hints will also be sent to the clients to help them out as time goes on.
  * Organizer - We can take inspiration from the myTunes project from earlier this semester and organize songs based on genre/title/author.
  * Library - This will work in tandem with the organizer, and will allow users to peruse the available list of songs.

## Division of work (we don't know who will do what yet)

  1. Music library — organization, management, sorting, etc.
  2. Music player using the command line
  3. Networking (for collaborative musical queue and musical game)
  4. Musical game — playing music on server, guessing on client
  5. Musical queue — playing music on server, adding music to queue on client
