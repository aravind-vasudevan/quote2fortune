`./quote2fortune.exe $1 > tutu`
fileName=$(grep -i FileName tutu | awk -F : '{print $2}')
echo "Filename is ["$fileName"]"
outputFileName=$fileName.dat
echo $outputFileName;
`strfile "$fileName" "$outputFileName"`
`sudo mv "$fileName" /usr/share/fortune/`
`sudo mv "$outputFileName" /usr/share/fortune/`
echo "Done copying files into fortunes folder. Try a fortune cookie by typing fortune "$fileName""
