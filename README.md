# **BRIDGE**
- [Test Game](#)
### I. GIỚI THIỆU GAME
  BRIDGE là 1 tựa game nói về nhân vật Hero giải cứu Crush của mình khỏi kẻ xấu. Tuy nhiên, trên con đường giải cứu có rất nhiều cạm bẫy, thử thách. Hero đã dũng cảm vượt qua gần hết, nhưng còn chặng cuối cùng
  rất khó khăn đó là leo qua các nền tảng khá cao mà ở dưới là các mối nguy hiểm đang rình rập, đe doạ. 
  Vậy nên người chơi hãy giúp Hero di chuyển đảm bảo không bị rơi để Hero cứu được Crush khỏi kẻ xấu.

  Game dựa trên ý tưởng từ game Monkey Stick: https://youtu.be/Op2xQs5VFmo?si=TQx7v1nbStfXMApl
### II. THAY ĐỔI MỚI DỰA TRÊN Ý TƯỞNG CŨ
  - Thêm tính năng level, các nút: _Back_, _Continue_.
  - Nhân vật có thể có thêm mạng sống hồi sinh bằng trái tim trên nền tảng, hiển thị số mạng sống trên màn hình.   
  - Tăng độ khó cho game bằng cách thêm 2 loại nền tảng mới: _Nền tảng biến mất_; _Nền tảng di chuyển_; có thể có sự kết hợp của cả 2 xuất hiện ở từng level theo xác suất.  
  - Cập nhật số lượng nền tảng, tỉ lệ xuất hiện 2 loại nền tảng mới tăng lên theo từng level.   
  - Thay đổi vị trí đặt gậy, vị trí đứng của nhân vật, lưu lại các level đã vượt qua, mở khoá level mới khi hoàn thành level hiện tại.  
  - Cập nhật lại đồ hoạ, âm thanh mới cho game.
### III. CÁCH CÀI ĐẶT GAME
  - Người chơi tải 4 thư viện SDL về và cài đặt SDL trên IDE đã có: [Picture](#)
  - 

### IV. HƯỠNG DẪN CHƠI GAME
  Người chơi phải chơi lần lượt từng level, nếu chưa qua level trước thì không thể mở khoá level sau. Chỉ cần vượt qua hết các level tức là đã dành chiến thắng và Hero cứu được Crush. Cụ thể:
  - Sau khi vào game và kích vào _Play Now_, sẽ dẫn chúng ta đến phần chọn level chơi:
  [Picture](#)
  - Ở phần lựa chọn level, người chơi bắt đầu từ level đầu tiên, sau khi thắng 1 level sẽ được lưu lại kết quả: [Picture](#)
  - Trong 1 level chơi, nhiệm vụ của người chơi là kéo dài cây gậy bắc cầu qua các nền tảng, không được kéo quá dài hoặc quá ngắn nhân vật sẽ bị rơi khi đi qua: [Picture](#)
  - Gậy có thuộc tính đặc biệt: Khi bắc cầu qua nền tảng di chuyển sẽ làm cho nền tảng dừng lại, giúp nhân vật dễ di chuyển hơn: [Picture](#)
  - Người chơi có thể tăng số mạng sống cho nhân vật hồi sinh bằng cách kéo gậy chạm đúng trái tim giữa nền tảng, nếu lệch sẽ không được: [Picture](#)   
  - Nền tảng sẽ gồm 3 loại:  
    1. _Nền tảng bình thường_: nền tảng dễ chơi nhất, người chơi chỉ việc kéo đủ chiều dài gậy để đi qua mà không gặp bất kì khó khăn nào: [Picture](#)
    2. _Nền tảng biến mất_: nền tảng này sẽ giới hạn thời gian đứng của nhân vật, nếu không di chuyển nhanh nhân vật sẽ bị rơi xuống khi nền tảng biến mất: [Picture](#)
    3. _Nền tảng di chuyển_: nền tảng này dịch chuyển quanh vị trí nó đứng, là nền tảng khó nhất, đòi hỏi người chơi phải linh hoạt trong việc kéo gậy di chuyển: [Picture](#)  
  - Ngoài ra, có thể có sự kết hợp ngẫu nhiên giữa nền tảng biến mất và nền tảng di chuyển, tức là vừa giới hạn thời gian đứng vừa dịch chuyển.
  - Khi chơi game:  
    +) Người chơi có thể quay lại bằng nút _Back_: [Picture](#)  
    +) Nếu muốn chơi lại màn chơi, chọn nút _Replay_: [Picture](#)  
    +) Sau khi thắng level chơi hiện tại, người chơi có thể tiếp tục level kế tiếp bằng nút _Continue_: [Picture](#)  
### V. NHỮNG NGUỒN SỬ DỤNG ĐỂ TẠO GAME
  - Background:  
    +) Popup: https://pngtree.com/freepng/popup-wooden-game_6839509.html  
    +) Main + MenuLevel: https://www.wallpaperflare.com/deer-and-trees-deer-in-forest-during-sunset-drawing-animals-wallpaper-cs  
    +) Playing: https://e0.pxfuel.com/wallpapers/468/274/desktop-wallpaper-eagle-landscape-mountains-minimalist-artist-background-and.jpg    
    +) Title + TitleLevel: Vẽ bởi AI.  
    +) Platform: Vẽ bởi AI.  
  - Character: https://www.shutterstock.com/vi/image-vector/ninja-game-sprites-suitable-side-scrolling-496110337  
  - ButtonGame:  
    +) Các ô level: Vẽ bởi AI  
    +) Các nút còn lại: https://pngtree.com/freepng/set-of-wood-game-buttons-icon-material_6002466.html  
  - Music:  
    +) Click: https://pixabay.com/vi/sound-effects/search/game%20%e1%ba%a5n%20n%c3%bat/  
    +) Playing: https://tiengdong.com/tieng-nuoc-chay-chim-hot  
    +) Main: https://pixabay.com/vi/music/search/genre/tr%c3%b2%20ch%c6%a1i%20video/?pagi=2  
    +) Walk: https://pixabay.com/vi/sound-effects/search/game%20nh%c3%a2n%20v%e1%ba%adt%20ch%e1%ba%a1y/?pagi=3  
    +) Fall: https://tiengdong.com/tieng-co-the-roi-xuong-sich; https://tiengdong.com/tieng-ten-lua-roi-meme  
    +) Win: https://tiengdong.com/am-thanh-gianh-chien-thang-trong-cac-tro-choi-dien-tu  
    +) StickLength: https://pixabay.com/vi/sound-effects/search/r%c6%a1i%20xu%e1%bb%91ng/?pagi=3  
    +) StickDown: https://pixabay.com/vi/sound-effects/search/90s/  
  - 1 số công cụ AI hỗ trợ khác như: ChatGPT, DeepSeek.
  - Chuyển âm thanh sang dạng wav.
  - Xoá phông nền hình ảnh.
  - Kết hợp sử dụng cả 4 thư viện SDL cho game: SDL, SDL_image, SDL_mixer, SDL_ttf.

