// --- DATABASE INITIALIZATION ---
const ADMIN_DEFAULT = [{name: "Administrator", id: 1, user: "admin", pass: "admin", role: "admin"}];

let users = JSON.parse(localStorage.getItem('users')) || ADMIN_DEFAULT;
let matakuliah = JSON.parse(localStorage.getItem('matakuliah')) || [];
let krsData = JSON.parse(localStorage.getItem('krs')) || [];
let nilaiData = JSON.parse(localStorage.getItem('nilai')) || [];

let currentUser = null;

// --- CORE FUNCTIONS ---
function handleLogin() {
    const u = document.getElementById('username').value;
    const p = document.getElementById('password').value;
    
    const user = users.find(x => x.user === u && x.pass === p);

    if (user) {
        currentUser = user;
        document.getElementById('login-screen').classList.add('hidden');
        document.getElementById('dashboard-screen').classList.remove('hidden');
        document.getElementById('user-display').innerText = user.name;
        
        // Tampilkan menu berdasarkan role
        document.querySelectorAll('.role-menu').forEach(m => m.classList.add('hidden'));
        if(user.role === 'admin') document.getElementById('admin-menu').classList.remove('hidden');
        if(user.role === 'dosen') document.getElementById('dosen-menu').classList.remove('hidden');
        if(user.role === 'mahasiswa') document.getElementById('mhs-menu').classList.remove('hidden');
        
        renderHome();
    } else {
        document.getElementById('login-msg').innerText = "Akses ditolak! Cek kembali akun Anda.";
    }
}

function renderHome() {
    document.getElementById('page-title').innerText = "Beranda";
    document.getElementById('dynamic-area').innerHTML = `
        <div class="card">
            <h1>Selamat Datang, ${currentUser.name}!</h1>
            <p style="margin-top:10px; color:#666">Anda login sebagai <b>${currentUser.role.toUpperCase()}</b>. Gunakan menu di samping untuk mengelola data akademik.</p>
        </div>
    `;
}

function showForm(type) {
    const area = document.getElementById('dynamic-area');
    let title = "";
    let html = `<div class="card">`;

    if(type === 'form-mhs' || type === 'form-dosen') {
        const isMhs = type === 'form-mhs';
        title = isMhs ? "Tambah Data Mahasiswa" : "Tambah Data Dosen";
        html += `<h3>Form ${title}</h3>
            <input type="text" id="f-nama" placeholder="Nama Lengkap">
            <input type="number" id="f-id" placeholder="${isMhs ? 'NIM' : 'NIDN'}">
            <input type="text" id="f-user" placeholder="Username">
            <input type="password" id="f-pass" placeholder="Password">
            <button onclick="saveUser('${isMhs ? 'mahasiswa' : 'dosen'}')">Simpan Akun</button>`;
    } 
    else if(type === 'form-mk') {
        title = "Mata Kuliah Baru";
        html += `<h3>Form Mata Kuliah</h3>
            <input type="text" id="mk-kode" placeholder="Kode MK">
            <input type="text" id="mk-nama" placeholder="Nama Mata Kuliah">
            <input type="number" id="mk-sks" placeholder="SKS">
            <input type="text" id="mk-hari" placeholder="Hari (Senin-Jumat)">
            <input type="text" id="mk-jam" placeholder="Jam (Contoh: 08:00)">
            <input type="text" id="mk-ruang" placeholder="Ruangan">
            <button onclick="saveMK()">Simpan Mata Kuliah</button>`;
    }
    else if(type === 'form-nilai') {
        title = "Input Nilai";
        html += `<h3>Input Nilai Mahasiswa</h3>
            <input type="number" id="n-nim" placeholder="NIM Mahasiswa">
            <input type="text" id="n-kode" placeholder="Kode MK">
            <input type="number" id="n-angka" placeholder="Nilai (0-100)">
            <button onclick="saveNilai()">Simpan Nilai</button>`;
    }

    html += `</div>`;
    area.innerHTML = html;
    document.getElementById('page-title').innerText = title;
}

// --- LOGIC FUNCTIONS (VALIDASI) ---

function saveUser(role) {
    const name = document.getElementById('f-nama').value;
    const id = parseInt(document.getElementById('f-id').value);
    const user = document.getElementById('f-user').value;
    const pass = document.getElementById('f-pass').value;

    if(users.some(u => u.id === id)) return alert("ID (NIM/NIDN) sudah terdaftar!");

    users.push({name, id, user, pass, role});
    localStorage.setItem('users', JSON.stringify(users));
    alert("Akun berhasil dibuat!");
    renderHome();
}

function saveMK() {
    const mk = {
        kode: document.getElementById('mk-kode').value,
        nama: document.getElementById('mk-nama').value,
        sks: parseInt(document.getElementById('mk-sks').value),
        hari: document.getElementById('mk-hari').value,
        jam: document.getElementById('mk-jam').value,
        ruang: document.getElementById('mk-ruang').value
    };

    // Validasi Bentrok (Logic C++: h == hari && j == jam && r == ruang)
    const bentrok = matakuliah.find(m => m.hari === mk.hari && m.jam === mk.jam && m.ruang === mk.ruang);
    if(bentrok) return alert(`Jadwal Bentrok dengan MK: ${bentrok.nama}`);

    matakuliah.push(mk);
    localStorage.setItem('matakuliah', JSON.stringify(matakuliah));
    alert("Mata kuliah berhasil ditambahkan!");
    renderHome();
}

function renderKRS() {
    document.getElementById('page-title').innerText = "Pengambilan KRS";
    let html = `<div class="card"><h3>Jadwal Tersedia</h3><table>
                <tr><th>Kode</th><th>Mata Kuliah</th><th>SKS</th><th>Jadwal</th><th>Aksi</th></tr>`;
    
    matakuliah.forEach(m => {
        const sudah = krsData.some(k => k.nim === currentUser.id && k.kode === m.kode);
        html += `<tr>
            <td>${m.kode}</td>
            <td>${m.nama}</td>
            <td>${m.sks}</td>
            <td>${m.hari}, ${m.jam} (${m.ruang})</td>
            <td>${sudah ? '✅' : `<button onclick="ambilMK('${m.kode}')" style="padding:5px 10px">Ambil</button>`}</td>
        </tr>`;
    });
    html += `</table></div>`;
    document.getElementById('dynamic-area').innerHTML = html;
}

function ambilMK(kode) {
    krsData.push({nim: currentUser.id, kode: kode});
    localStorage.setItem('krs', JSON.stringify(krsData));
    renderKRS();
}

function renderNilai() {
    document.getElementById('page-title').innerText = "Transkrip Nilai";
    let totalBobot = 0, totalSKS = 0;
    let html = `<div class="card"><table><tr><th>MK</th><th>SKS</th><th>Nilai</th><th>Bobot</th></tr>`;

    nilaiData.filter(n => n.nim === currentUser.id).forEach(n => {
        const mk = matakuliah.find(m => m.kode === n.kode);
        if(mk) {
            let bobot = n.angka >= 85 ? 4 : n.angka >= 75 ? 3 : n.angka >= 65 ? 2 : n.angka >= 55 ? 1 : 0;
            totalBobot += (bobot * mk.sks);
            totalSKS += mk.sks;
            html += `<tr><td>${mk.nama}</td><td>${mk.sks}</td><td>${n.angka}</td><td>${bobot.toFixed(1)}</td></tr>`;
        }
    });

    const ipk = totalSKS > 0 ? (totalBobot / totalSKS).toFixed(2) : "0.00";
    html += `</table><h2 style="margin-top:20px">IP Kumulatif (IPK): ${ipk}</h2></div>`;
    document.getElementById('dynamic-area').innerHTML = html;
}

function saveNilai() {
    const nim = parseInt(document.getElementById('n-nim').value);
    const kode = document.getElementById('n-kode').value;
    const angka = parseInt(document.getElementById('n-angka').value);

    if(!krsData.some(k => k.nim === nim && k.kode === kode)) return alert("Mahasiswa belum KRS MK ini!");

    const idx = nilaiData.findIndex(n => n.nim === nim && n.kode === kode);
    if(idx !== -1) nilaiData[idx].angka = angka;
    else nilaiData.push({nim, kode, angka});

    localStorage.setItem('nilai', JSON.stringify(nilaiData));
    alert("Data nilai berhasil masuk sistem!");
}

function logout() { location.reload(); }

