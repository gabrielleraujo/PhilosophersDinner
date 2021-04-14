#### PhilosophersDinner
##### O "Jantar dos Filosofos" é um problema clássico de sincronização, que foi proposto em 1965 pelo matemático Dijkstra.
![img](https://3.bp.blogspot.com/-sLA2YBZ42a0/Vt9w3Qz2K5I/AAAAAAAADg0/993iZi5XD2A/s1600/jantar-filosofos.jpg)
##### Resumo do problema:
- Temos n filósofos e n talheres.
- Os filósofos comem, pensam e ficam com fome alternadamente.
- Entre 2 pratos há 1 talher. 
- Um filósofo só pode comer se os 2 talheres ao lado de seu prato estiverem vagos.
- Um filósofo não pode comer se algum dos seus vizinhos adjacentes estiverem comendo.

Em termos da programação, isso significa que teremos n (filósofos) e o mesmo número n de (talheres). E para o compartilhamento desses múltiplos talheres para os filósofos de modo em que eles possam comer precisamos usar uma programação simultânea, e por isso usamos Threads.
