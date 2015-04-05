require 'serialport'
require 'appscript'
require 'io/wait'

sp = SerialPort.new "/dev/cu.usbmodem411", 9600

spot = Appscript.app('Spotify')
spot.launch

sp.write("cls,cls\n")

time = Time.now

lastCmd = '0'

while !sp.ready?
  sleep(0.1)
end
sleep(2)

update = true
artist = ''
song = ''

while true

  curTime = Time.now

  if curTime > time + 2 || update
    time = curTime
    
    cur_artist = spot.current_track.artist.get
    cur_song = spot.current_track.name.get
    
    if cur_artist != artist && cur_song != song
      artist = cur_artist
      song = cur_song
      puts "#{artist},#{song}"
      sp.write("#{artist},#{song}\n")
    end

    update = false
  end

  if sp.ready?
    curCmd = sp.gets.chomp
    puts curCmd
    if curCmd != lastCmd
      if lastCmd == '5'
        spot.previous_track
        update = true
      elsif lastCmd == '4'
        spot.next_track
        update = true
      elsif lastCmd == '3'
        curVol = spot.sound_volume.get
        spot.sound_volume.set(curVol - 10)
      elsif lastCmd == '2'
        curVol = spot.sound_volume.get
        spot.sound_volume.set(curVol + 10)
      elsif lastCmd == '1'
        spot.playpause
      end
    end
    lastCmd = curCmd
  end

  sleep(0.001)
end
