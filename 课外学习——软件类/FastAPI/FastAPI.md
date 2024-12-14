# FastAPI中的QuickStart


```
from fastapi import FastAPI
import uvicorn

app = FastAPI()

@app.get("/")
async def home():
    return {"user_id": 1001} #创建返回值

@app.get("/shop")
async def home():
    return {"shop_id": 1002} #创建返回值

#不使用终端命令的uvicorn而直接点击运行的方式
if __name__ == "__name__":
    uvicorn.run("1.py:app", port = 8080, debug = True, reload = True)
```

# FastAPI中的路径操作装饰器方法参数简介
## 路径操作装饰器
FastAPI中的请求方法有：
```
@app.get()
@app.post()
@app.put()
@app.patch()
@app.delete()
@app.options()
@app.head()
@app.trace()
```
路径操作装饰器方法的参数主要包括了`respond_model` `status_code` `tags`等，具体为：
```
@app.post(
    "items/{item_id}",
    respond_mode = Item,
    status_model = status.HTTP_200_OK,
    tags = ["AAA"],
    summary = "items's summary",
    description = "items's description",
    response_description = "item's response_description",
    deprecated = False.
)

```
可以尝试运行的例子为：
```
from fastapi import FastAPI
import uvicorn

app = FastAPI()

@app.post("/items", tags = ["itmes测试接口“]， summary = ["itmes测试总结“])
async def home():
    return {"user_id": 1001} #创建返回值

#不使用终端命令的uvicorn而直接点击运行的方式
if __name__ == "__name__":
    uvicorn.run("1.py:app", port = 8080, debug = True, reload = True)
```

## include_router
include_router功能是将不同子应用的路由作分发和解耦。应用的场景是在工程特别大的时候，有许多的子应用的情况。<br>
在main.py主文件中构建以下代码：
```
from fastapi import FastAPI
import uvicorn

app = FastAPI()

app.include_router("")

if __main__ == "__main__":
    uvicorn.run("main:app", port = 8080, debug = True, reload = True)

```
随后，构建一个新的文件夹叫Apps，里面分别构建`App01.py`和`App02.py`。
在App01.py文件中，构建以下代码：
```
from fastapi import APIRouter

App01 = APIRouter()

@App01.get("/App01/food")
def App01_food():
    return {"App01": "food"}

@App01.get("/App01/bed")
def App01_bed():
    return {"App01": "bed"}
```

在App02.py文件中，构建以下代码：
```
from fastapi import APIRouter

App02 = APIRouter()

@App02.get("/App02/fruit")
def App01_food():
    return {"App02": "fruit"}

@App02.get("/App02/shoose")
def App01_bed():
    return {"App02": "shoose"}
```
