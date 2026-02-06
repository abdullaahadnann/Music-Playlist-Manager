const addBtn = document.getElementById('addBtn');
const playBtn = document.getElementById('playBtn');
const nextBtn = document.getElementById('nextBtn');
const prevBtn = document.getElementById('prevBtn');
const deleteBtn = document.getElementById('deleteBtn');
const playlistEl = document.getElementById('playlist');
const audio = document.getElementById('audio');

let playlist = [];
let currentIndex = -1;

// Add Song
addBtn.addEventListener('click', () => {
    const title = document.getElementById('title').value.trim();
    const artist = document.getElementById('artist').value.trim() || 'Unknown';
    const fileInput = document.getElementById('file');
    const file = fileInput.files[0];

    if (!title || !file) {
        alert("Please enter title and choose a file!");
        return;
    }

    const url = URL.createObjectURL(file);

    playlist.push({ title, artist, url });
    renderPlaylist();

    // Clear inputs
    document.getElementById('title').value = '';
    document.getElementById('artist').value = '';
    fileInput.value = '';
});

// Render Playlist
function renderPlaylist() {
    playlistEl.innerHTML = '';
    playlist.forEach((song, index) => {
        const li = document.createElement('li');
        li.textContent = `${song.title} - ${song.artist}`;
        li.classList.toggle('active', index === currentIndex);
        li.addEventListener('click', () => {
            currentIndex = index;
            playSong();
        });
        playlistEl.appendChild(li);
    });
}

// Play Song
function playSong() {
    if (currentIndex < 0 || currentIndex >= playlist.length) return;
    audio.src = playlist[currentIndex].url;
    audio.play();
    renderPlaylist();
}

// Next Song
nextBtn.addEventListener('click', () => {
    if (playlist.length === 0) return;
    currentIndex = (currentIndex + 1) % playlist.length;
    playSong();
});

// Previous Song
prevBtn.addEventListener('click', () => {
    if (playlist.length === 0) return;
    currentIndex = (currentIndex - 1 + playlist.length) % playlist.length;
    playSong();
});

// Play/Pause toggle
playBtn.addEventListener('click', () => {
    if (!audio.src) return;
    if (audio.paused) audio.play();
    else audio.pause();
});

// Delete Song
deleteBtn.addEventListener('click', () => {
    if (currentIndex < 0) return;
    playlist.splice(currentIndex, 1);
    if (currentIndex >= playlist.length) currentIndex = playlist.length - 1;
    playSong();
});
