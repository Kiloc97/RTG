from flask import Flask, request, jsonify
from flask_restful import Resource, Api
from flask_jwt_extended import (
    JWTManager, create_access_token,
    jwt_required, get_jwt_identity
)

app = Flask(__name__)
app.config['JWT_SECRET_KEY'] = 'super-secret-key'
api = Api(app)
jwt = JWTManager(app)

# 임시 메모리 DB
users = {}
books = {}
loans = []

# 루트 테스트용
@app.route('/')
def index():
    return "Library API Server Running!"

# 회원가입 엔드포인트
class Signup(Resource):
    def post(self):
        data = request.get_json()
        username = data.get('username')
        if username in users:
            return {'msg': '이미 존재하는 사용자'}, 400
        users[username] = {
            'email': data.get('email'),
            'password': data.get('password'),
            'full_name': data.get('full_name')
        }
        return {'msg': '회원가입 성공'}, 201

# 로그인(JWT 토큰 발급)
class Login(Resource):
    def post(self):
        data = request.get_json()
        user = users.get(data.get('username'))
        if not user or user['password'] != data.get('password'):
            return {'msg': '잘못된 정보'}, 401
        token = create_access_token(identity=data.get('username'))
        return {'access_token': token}, 200

# 도서 등록 (관리자 전용 추가 가능)
class BookAdd(Resource):
    @jwt_required()
    def post(self):
        data = request.get_json()
        book_id = len(books) + 1
        books[book_id] = {
            'id': book_id,
            'title': data.get('title'),
            'author': data.get('author'),
            'isbn': data.get('isbn'),
            'category': data.get('category'),
            'total_copies': data.get('total_copies'),
            'available': True
        }
        return books[book_id], 201

# 도서 조건 검색
class BookList(Resource):
    def get(self):
        category = request.args.get('category')
        available = request.args.get('available')
        result = []
        for book in books.values():
            if category and book['category'] != category:
                continue
            if available and str(book['available']).lower() != available.lower():
                continue
            result.append(book)
        return result, 200

# 대출
class Borrow(Resource):
    @jwt_required()
    def post(self):
        data = request.get_json()
        book_id = data.get('book_id')
        curr_user = get_jwt_identity()
        book = books.get(book_id)
        if not book or not book['available']:
            return {'msg': '대출 불가'}, 400
        book['available'] = False
        loans.append({'book_id': book_id, 'user_id': curr_user})
        return {'msg': '대출 완료'}, 200

# 대출 내역
class MyLoans(Resource):
    @jwt_required()
    def get(self):
        curr_user = get_jwt_identity()
        my_loans = [loan for loan in loans if loan['user_id'] == curr_user]
        return my_loans, 200

# RESTful 엔드포인트 등록
api.add_resource(Signup, '/auth/signup')
api.add_resource(Login, '/auth/login')
api.add_resource(BookAdd, '/books')
api.add_resource(BookList, '/books')
api.add_resource(Borrow, '/loans')
api.add_resource(MyLoans, '/users/me/loans')

if __name__ == '__main__':
    app.run(debug=True)
