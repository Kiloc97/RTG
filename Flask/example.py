import requests

base_url = "http://localhost:5000"

# 회원가입
signup_data = {
    "username": "john_doe",
    "email": "john@example.com",
    "password": "securepass123",
    "full_name": "John Doe"
}
response = requests.post(f"{base_url}/auth/signup", json=signup_data)
print(response.json())

# 로그인 → access_token 확보
login_data = {"username": "john_doe", "password": "securepass123"}
auth_response = requests.post(f"{base_url}/auth/login", json=login_data)
token = auth_response.json().get("access_token")
headers = {"Authorization": f"Bearer {token}"}

# 책 등록
book_data = {
    "title": "Python Programming",
    "author": "John Smith",
    "isbn": "978-0123456789",
    "category": "Programming",
    "total_copies": 5
}
book_resp = requests.post(f"{base_url}/books", json=book_data, headers=headers)
print(book_resp.json())

# 책 검색(예: category=Programming, available=true)
search_resp = requests.get(f"{base_url}/books?category=Programming&available=true")
print(search_resp.json())

# 대출 요청
borrow_data = {"book_id": 1}
borrow_resp = requests.post(f"{base_url}/loans", json=borrow_data, headers=headers)
print(borrow_resp.json())

# 내 대출 내역 조회
loans_resp = requests.get(f"{base_url}/users/me/loans", headers=headers)
print(loans_resp.json())
