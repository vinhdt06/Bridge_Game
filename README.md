# **BRIDGE**
- Review & Test Game: https://drive.google.com/file/d/1aVck9cOmNtZ9QGeJMoj8VavnD4nHE5QB/view?usp=drive_link  
### I. GIỚI THIỆU GAME
  BRIDGE là 1 tựa game nói về nhân vật Hero giải cứu Crush của mình khỏi kẻ xấu. Tuy nhiên, trên con đường giải cứu có rất nhiều cạm bẫy, thử thách. Hero đã dũng cảm vượt qua gần hết, nhưng còn chặng cuối cùng
  rất khó khăn đó là leo qua các nền tảng khá cao mà ở dưới là các mối nguy hiểm đang rình rập, đe doạ. 
  Vậy nên người chơi hãy giúp Hero di chuyển đảm bảo không bị rơi để Hero cứu được Crush khỏi kẻ xấu.

  Game dựa trên ý tưởng từ game Monkey Stick: https://youtu.be/Op2xQs5VFmo?si=TQx7v1nbStfXMApl
### II. THAY ĐỔI MỚI DỰA TRÊN Ý TƯỞNG CŨ
  - Thêm tính năng level, các nút: _Back_, _Continue_.
  - Tăng độ khó cho game bằng cách thêm 2 loại nền tảng mới: _Nền tảng biến mất_; _Nền tảng di chuyển_; có thể có sự kết hợp của cả 2 xuất hiện ở từng level theo xác suất.  
  - Cập nhật số lượng nền tảng, tỉ lệ xuất hiện 2 loại nền tảng mới tăng lên theo từng level.
  - Nhân vật có thể có thêm mạng sống hồi sinh bằng trái tim trên nền tảng, hiển thị số mạng sống trên màn hình.   
  - Thay đổi vị trí đặt gậy, vị trí đứng của nhân vật, lưu lại các level đã vượt qua, mở khoá level mới khi hoàn thành level hiện tại.  
  - Cập nhật lại đồ hoạ, âm thanh mới cho game, thêm tính nổi cho nút.
### III. CÁCH CÀI ĐẶT GAME
  - Người chơi click vào file _Bridge.exe_:
  
    ![Screenshot 2025-04-25 235218](https://github.com/user-attachments/assets/fdedd5a7-1ac8-40ca-9504-3f52b33440ee)  
    
  - Tiếp theo click vào _Raw_ để tải về file:  
  
    ![Screenshot 2025-04-24 205316](https://github.com/user-attachments/assets/885c7055-e768-483a-b247-097f9031842f)  

  - Giải nén file, sau đó vào file và chơi game ở phần _TESTGAME2_.  

### IV. HƯỠNG DẪN CHƠI GAME
  Người chơi phải chơi lần lượt từng level, nếu chưa qua level trước thì không thể mở khoá level sau. Chỉ cần vượt qua hết các level tức là đã dành chiến thắng và Hero cứu được Crush. Cụ thể:
  - Sau khi vào game và kích vào _Play Now_, sẽ dẫn chúng ta đến phần chọn level chơi:
      
    ![Screenshot 2025-04-25 235030](https://github.com/user-attachments/assets/9204908c-c78b-4c7b-9328-519b2b5b4795)
  
    - Main Menu Game với nút _Play Now_.
        
    ![Screenshot 2025-04-20 232152](https://github.com/user-attachments/assets/3ef97e40-cb62-4cf2-a0fa-64f07e862bc8)  
    - Phần lựa chọn level.  
    
  - Ở phần lựa chọn level, người chơi bắt đầu từ level đầu tiên, sau khi thắng 1 level sẽ được lưu lại kết quả:

    ![Screenshot 2025-04-20 232645](https://github.com/user-attachments/assets/0ad9c4d8-234e-423b-b0aa-88a9bdfef30f)
    - Ở hình ảnh level 1 đã vượt qua sẽ được tô đậm, sau đó level 2 được mở (tô mờ) tiếp theo để chơi, các level khác vẫn trong trạng thái bị khoá.  
  
  - Trong 1 level chơi, nhiệm vụ của người chơi là click giữ chuột vào màn hình game để kéo dài cây gậy bắc cầu qua các nền tảng, không được kéo quá dài hoặc quá ngắn nhân vật sẽ bị rơi khi đi qua:

      ![Screenshot 2025-04-20 233807](https://github.com/user-attachments/assets/549b1d7d-0fe4-48b7-9cfc-54951ca0b759)  
  
  - Gậy có thuộc tính đặc biệt: Khi bắc cầu qua nền tảng di chuyển sẽ làm cho nền tảng dừng lại, giúp nhân vật dễ di chuyển hơn.  
  - Người chơi có thể tăng số mạng sống cho nhân vật hồi sinh bằng cách kéo gậy chạm đúng trái tim giữa nền tảng, nếu lệch thì sẽ không có mạng hay nếu chạm đúng trái tim nhưng quá chậm so với bị rơi thì cũng không được chấp nhận:  

      ![Screenshot 2025-04-20 235046](https://github.com/user-attachments/assets/2b54637e-fe0b-40d9-820e-4317646dae57)
    
      ![Screenshot 2025-04-20 235916](https://github.com/user-attachments/assets/f1567ca4-134e-4187-a012-a489749d2055)
      - Nếu đặt chính xác sẽ có thêm mạng và được cập nhật số mạng trên màn hình.  

  - Game đã được fix lỗi sẽ không thể kéo gậy dài để vượt qua đồng thời nhiều nền tảng, nhân vật vẫn sẽ bị rơi dù có đặt đúng:
      
    ![Screenshot 2025-04-25 233333](https://github.com/user-attachments/assets/1d9e6b1b-4c69-4fd6-a624-acb5d5a109e8)  

    
  - Nền tảng sẽ gồm 3 loại:  
    1. _Nền tảng bình thường_: nền tảng dễ chơi nhất, người chơi chỉ việc kéo đủ chiều dài gậy để đi qua mà không gặp bất kì khó khăn nào:
  
       ![Screenshot 2025-04-20 233236](https://github.com/user-attachments/assets/0fc7941e-c40c-47c9-b97e-4b695424a54d)  
  
    3. _Nền tảng biến mất_: nền tảng này sẽ giới hạn thời gian đứng của nhân vật, nếu không di chuyển nhanh nhân vật sẽ bị rơi xuống khi nền tảng biến mất:
  
       ![Screenshot 2025-04-20 233122](https://github.com/user-attachments/assets/e2576d0d-b89b-4c78-bd54-c7958c1b3bdb)  

    4. _Nền tảng di chuyển_: nền tảng này dịch chuyển quanh vị trí nó đứng, là nền tảng khó nhất, đòi hỏi người chơi phải linh hoạt trong việc kéo gậy di chuyển:

       ![Screenshot 2025-04-20 235501](https://github.com/user-attachments/assets/0378c38a-1596-45a4-9eab-a2210eb864b8)  

  - Ngoài ra, có thể có sự kết hợp ngẫu nhiên giữa nền tảng biến mất và nền tảng di chuyển, tức là vừa giới hạn thời gian đứng vừa dịch chuyển.
  - Khi chơi game:  
    +) Người chơi có thể quay lại bằng nút _Back_:

      ![Screenshot 2025-04-20 233457](https://github.com/user-attachments/assets/c7e77adc-1ec0-4501-9542-4f875ac5ddc6)  
  
    +) Nếu muốn chơi lại màn chơi, chọn nút _Replay_:

      ![Screenshot 2025-04-20 233549](https://github.com/user-attachments/assets/f4d3d831-b2dc-497a-a0fd-6434ea6d5ca0)  
  
    +) Sau khi thắng level chơi hiện tại, người chơi có thể tiếp tục level kế tiếp bằng nút _Continue_:

       ![Screenshot 2025-04-20 233643](https://github.com/user-attachments/assets/bb980f89-f451-4446-aa12-118ba29c9d45)
  
    +) Nút âm thanh cho game:

      - Mở: ![Screenshot 2025-04-25 234445](https://github.com/user-attachments/assets/0ddc9f82-387b-4ce4-aa70-160c302748fd)

      - Tắt: ![Screenshot 2025-04-25 234452](https://github.com/user-attachments/assets/4f025446-bce1-4cf8-b8f9-d446f0c8760e)  

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

