
--[[

    Author: Ark223
    Version: 2.0.1
    Date: 28th October 2023
    Copyright (c) 2023 Arkadiusz Kwiatkowski

    [License]
    Use, modification & distribution is subject to Boost Software License Ver 1.
    https://www.boost.org/LICENSE_1_0.txt

    [API]
    Matrix - two-dimensional arrays of numbers
    Matrix:new(...) - initial call, returns instance
        arguments:
            * data [table] - holds the data stored in the matrix
            * rows [number] - sets the number of rows of the matrix
            * cols [number] - sets the number of columns of the matrix
        overloads:
            * () [null matrix] => Matrix({{}}, 0, 0)
            * ({...}) [table] => Matrix({...}, M, N)
            * ({...}, M, N) [table, number, number] => Matrix(...)
        metamethods:
            * Matrix [+|-|*|/|^] Matrix (matrix-matrix operation)
            * Matrix [+|-|*|/|^] number (matrix-scalar operation)
            * number [+|-|*|/|^] Matrix (scalar-matrix operation)
            * -Matrix (negation of elements in the matrix)
            * tostring(Matrix) (text representation of a matrix)
        functions:
            :abs() - computes the absolute value of each element within the matrix
            :all(func [function]) - indicates whether condition holds true for all elements
            :any(func [function]) - indicates whether condition holds true for any element
            :argmax(axis [0/1/nil]) - finds the index of maximum value along an axis/matrix
            :argmin(axis [0/1/nil]) - finds the index of minimum value along an axis/matrix
            :cosh() -- computes the hyperbolic cosine of each element within the matrix
            :diag(k [number]) - extracts a diagonal from the matrix with an optional offset
            :dot(other [Matrix]) - performs dot product between the current and another matrix
            .empty(rows [number], cols [number]) - returns an empty matrix with specified size
            :exp() - computes the exponential (e^x) of each element within the matrix
            :flatten(axis [0/1/nil]) - flattens the matrix into vector along an axis
            :get(row [number], col [number]) - retrieves the element from the matrix
            .init(row [number, col [number], func [function]) -- initializes a matrix,
                where the elements are generated based on provided initialization function
            :log() - computes the natural logarithm of each element within the matrix
            :map(func [function]) - creates a new matrix where each element is derived
                from applying a given function to the corresponding element in the matrix
            :max(axis [0/1/nil], seed [number/nil]) - finds the maximum value within the
                matrix, either along an axis or across the entire matrix (seed is optional)
            :maximum(obj [Matrix/number]) - creates a new matrix where each element is
                the maximum of the corresponding element in the original matrix and
                either a constant value (obj) or the elements of another matrix (obj)
            :mean(axis [0/1/nil]) - returns the mean value of elements along an axis/matrix
            :min(axis [0/1/nil], seed [number/nil]) - finds the minimum value within the
                matrix, either along an axis or across the entire matrix (seed is optional)
            :minimum(obj [Matrix/number]) - creates a new matrix where each element is
                the minimum of the corresponding element in the original matrix and
                either a constant value (obj) or the elements of another matrix (obj)
            .ones() - creates a new matrix where all elements are set to the value 1
            :ones_like() - creates a new matrix with the same dimensions as the
                original matrix, where all elements are initialized to the value 1
            :product(axis [0/1/nil]) - computes the product of values in the matrix
            :ptp(axis [0/1/nil]) - computes the peak-to-peak range of values in the matrix
            .randn(rows [number], cols [number], mean [number], std [number]) - creates and
                returns a matrix with randomized values generated from a standard normal
                distribution (default value of mean is 0 and of standard deviation is 1)
            .random(rows [number], cols [number]) - creates and returns a matrix with
                random values generated from a uniform distribution between 0 and 1
            .range(start [number], stop [number], step [number]) - creates and returns
                a matrix containing a sequence of values within a specified value range
            :reduce(axis [0/1/nil], init [number], func [function]) - performs a reduction
                operation on the matrix along a specified axis or across the entire matrix
            :reshape(rows [number], cols [number]) - reshapes the original matrix into a new
                matrix with different dimensions while preserving the original data order
            :set(row [number], col [number], val [number]) - sets the value in the matrix
            :sign() - creates a new matrix with signs of the corresponding matrix elements
            :sinh() - computes the hyperbolic sine of each element within the matrix
            :sqrt() - computes the square root of each element within the matrix
            :std(axis [0/1/nil]) - computes the standard deviation of the values
            :sub(start_row [number], end_row [number], start_col [number], end_col
                [number]) - extracts a submatrix from the original matrix based on indices
            :sum(axis [0/1/nil]) - computes the sum of the values along an axis/matrix
            :tanh() - computes the hyperbolic tangent of each element within the matrix
            :tranpose() - creates a new matrix that is the transpose of the original matrix
            :var(axis [0/1/nil]) - computes the variance of the values along an axis/matrix
            .zeros() - creates a new matrix where all elements are set to the value 0
            :zeros_like() - creates a new matrix with the same dimensions as the
                original matrix, where all elements are initialized to the value 0

    NeuralNetwork - contains a series of connected layers; to train the network it
        performs feed-forward operation to get the outputs of the layers and then
        backpropagates to calculate the gradients and update the weights and biases
    NeuralNetwork:New({...}) - initial call, returns instance
        arguments:
            * layers [table<Layer>] - represents the architecture of the neural network
            * epochs [number] - defines a number of training epochs (iterations) for learning
            * batch_size [number] - determines the batch size used for mini-batch training
            * loss [Loss] - represents the loss function to compute the error of prediciton
            * accuracy [Accuracy] - represents the accuracy metric used to evaluate the model
            * optimizer [Optimizer] - represents the optimization algorithm for model updates
        functions:
            :Accuracy(target [Matrix], pred [Matrix]) - calculates the accuracy of the model
            :LoadWeights(string [string]) - loads weights and biases from a serialized string
            :Loss(target [Matrix], pred [Matrix]) - calculates the loss between two outputs
            :Predict(input [Matrix]) - returns the prediction based on given input of values
            :SaveWeights() - returns the string representing weights and biases of the model
            :Train(input [Matrix], target [Matrix]) - performs a training over a dataset

--]]

math.randomseed(os.time())
local load = load or loadstring
local unpack = unpack or table.unpack
local tostring = tostring
local flush = io.flush
local write = io.write
local abs = math.abs
local ceil = math.ceil
local cos = math.cos
local exp = math.exp
local floor = math.floor
local huge = math.huge
local log = math.log
local max = math.max
local min = math.min
local pi = math.pi
local random = math.random
local sqrt = math.sqrt
local format = string.format
local rep = string.rep
local concat = table.concat
local insert = table.insert
local sort = table.sort
local prnt = function(str)
    write(str); flush(str) end
local sign = function(x) return x > 0
    and 1 or x < 0 and -1 or 0 end

-- Extra math functions
-- https://hewgill.com/picomath/lua/erf.lua.html
-- http://lua-users.org/wiki/HyperbolicFunctions

local function erf(x)
    local a = abs(x)
    local a1 = 0.254829592
    local a2 = -0.284496736
    local a3 = 1.421413741
    local a4 = -1.453152027
    local a5 = 1.061405429
    local p = 0.3275911
    local t = 1 / (1 + p * a)
    local sign = x < 0 and -1 or 1
    local y = ((((a5 * t + a4) * t
        + a3) * t + a2) * t + a1) * t
    return sign * (1 - y * exp(-a * a))
end

local function cosh(x)
    local x = x
    if x == 0 then return 1.0 end
    if x < 0 then x = -x end
    x = exp(x)
    x = x / 2.0 + 0.5 / x
    return x
end

local function sinh(x)
    local x, n = x, false
    if x == 0 then return 0.0 end
    if x < 0 then x, n = -x, true end
    if x < 1.0 then
        local y = x * x
        x = x + x * y *
            (((-0.78966127417357099479e0  * y +
                -0.16375798202630751372e3) * y +
                -0.11563521196851768270e5) * y +
                -0.35181283430177117881e6) /
            (((0.10000000000000000000e1 * y +
                -0.27773523119650701667e3) * y +
                0.36162723109421836460e5) * y +
                -0.21108770058106271242e7)
    else
        x = exp(x)
        x = x / 2.0 - 0.5 / x
    end
    if n then x = -x end
    return x
end

local function tanh(x)
    local x, n = x, false
    if x == 0 then return 0.0 end
    if x < 0 then x, n = -x, true end
    if x < 0.54930614433405 then
        local y = x * x
        x = x + x * y *
            ((-0.96437492777225469787e0  * y +
            -0.99225929672236083313e2) * y +
            -0.16134119023996228053e4) /
            (((0.10000000000000000000e1  * y +
                0.11274474380534949335e3) * y +
                0.22337720718962312926e4) * y +
                0.48402357071988688686e4)
    else
        x = exp(x)
        x = 1.0 - 2.0 / (x * x + 1.0)
    end
    if n then x = -x end
    return x
end

-- Class constructor and data serialization

local Class = function(...)
    local cls, bases = {}, {...}
    for _, base in ipairs(bases) do
        for param, value in pairs(base) do
            cls[param] = value
        end
    end
    cls.__index = cls
    function cls:new(...)
        local instance = setmetatable({}, cls)
        cls.__init(instance, ...)
        return instance
    end
    cls.__call = function(_, ...) return cls:new(...) end
    return setmetatable(cls, {__call = cls.__call})
end

local function Serialize(tbl, level)
    local level = level or 0
    local function serialize(value, depth)
        local otype = type(value)
        if otype == "number" then
            return tostring(value)
        elseif otype == "string" then
            return format("%q", value)
        elseif otype == "table" then
            local result = {}
            for key, val in pairs(value) do
                local s_key = "[" .. (type(key) ==
                    "string" and "\"" .. key .. "\""
                    or serialize(key, depth + 1)) .. "]"
                local s_val = serialize(val, depth + 1)
                insert(result, s_key .. "=" .. s_val)
            end
            local res = "{" .. concat(result, ",") .. "}"
            return (depth == 0 and "return " or "") .. res
        end
    end
    return serialize(tbl, level)
end

--------------------
------ Matrix ------

local Matrix = Class()

function Matrix:__init(data, rows, cols)
    self.data = data or {{}}
    self.rows = rows or #self.data or 0
    self.cols = cols or #self.data[1] or 0
    self.size = self.rows * self.cols
end

local function Iterator(m, n, inf)
    local i, j = 1, 1
    return function()
        if i > m then
            if inf then i = 1
            else return nil end
        end
        local c_i = i
        local c_j = j
        local r = false
        j = j + 1
        if j > n then
            r = true
            i = i + 1
            j = 1
        end
        return c_i, c_j, r
    end
end

function Matrix.__add(a, b)
    return Matrix.calculate(a, b,
        function(p, r) return p + r end)
end

function Matrix.__sub(a, b)
    return Matrix.calculate(a, b,
        function(p, r) return p - r end)
end

function Matrix.__mul(a, b)
    return Matrix.calculate(a, b,
        function(p, r) return p * r end)
end

function Matrix.__div(a, b)
    return Matrix.calculate(a, b,
        function(p, r) return p / r end)
end

function Matrix.__pow(a, b)
    return Matrix.calculate(a, b,
        function(p, r) return p ^ r end)
end

function Matrix:__unm()
    return self:map(function(x) return -x end)
end

function Matrix:__tostring()
    local rows = self.rows
    local cols = self.cols
    local widths = {}
    for j = 1, cols do
        widths[j] = 1
        for i = 1, rows do
            local len = #tostring(self.data[i][j])
            widths[j] = max(widths[j], len)
        end
    end
    local str = "["
    for i = 1, rows do
        str = str .. (i > 1 and " " or "") .. "["
        for j = 1, cols do
            local pad = j < cols and " " or ""
            local pat = "%-" .. widths[j] .. "s%s"
            str = str .. pat:format(self.data[i][j], pad)
        end
        str = str .. "]" .. (i < rows and "\n" or "")
    end
    return str .. "]"
end

function Matrix:abs()
    return self:map(function(x) return abs(x) end)
end

function Matrix:all(func)
    local rows, cols = self.rows, self.cols
    for row, col in Iterator(rows, cols) do
        local value = self.data[row][col]
        if not func(value, row, col) then
            return false
        end
    end
    return true
end

function Matrix:any(func)
    local rows, cols = self.rows, self.cols
    for row, col in Iterator(rows, cols) do
        local value = self.data[row][col]
        if func(value, row, col) then
            return true
        end
    end
    return false
end

function Matrix:argmax(axis)
    local max_val = -huge
    return self:reduce(axis, 0, function(s, x, i, j)
        if axis == 0 and i == 1 then max_val = -huge end
        if axis == 1 and j == 1 then max_val = -huge end
        if max_val >= x then return s end; max_val = x
        return axis == 0 and i or axis == 1 and j or
            axis == nil and (i - 1) * self.cols + j
    end)
end

function Matrix:argmin(axis)
    local min_val = huge
    return self:reduce(axis, 0, function(s, x, i, j)
        if axis == 0 and i == 1 then min_val = huge end
        if axis == 1 and j == 1 then min_val = huge end
        if min_val <= x then return s end; min_val = x
        return axis == 0 and i or axis == 1 and j or
            axis == nil and (i - 1) * self.cols + j
    end)
end

function Matrix.calculate(a, b, op)
    local data = {{}}
    local a_num = type(a) == "number"
    local b_num = type(b) == "number"
    local a_rows = a_num and 1 or a.rows
    local b_rows = b_num and 1 or b.rows
    local a_cols = a_num and 1 or a.cols
    local b_cols = b_num and 1 or b.cols
    for row = 1, max(a_rows, b_rows) do
        data[row] = data[row] or {}
        local ra = a_num and {} or nil
        local rb = b_num and {} or nil
        ra = ra or a.data[row] or a.data[1]
        rb = rb or b.data[row] or b.data[1]
        for col = 1, max(a_cols, b_cols) do
            local va = ra[col] or ra[1] or a
            local vb = rb[col] or rb[1] or b
            data[row][col] = op(va, vb)
        end
    end
    return Matrix:new(data)
end

function Matrix:cosh()
    return self:map(function(x) return cosh(x) end)
end

function Matrix:diag(k)
    local k = k or 0
    local diag_values = {}
    local start_row, start_col
    local end_row, end_col
    if k >= 0 then
        start_row = 1
        start_col = 1 + k
        end_row = self.rows - k
        end_col = self.cols
    else
        start_row = 1 - k
        start_col = 1
        end_row = self.rows
        end_col = self.cols + k
    end
    for i = start_row, end_row do
        diag_values[i] = self.data[i][i + k]
    end
    return Matrix:new({diag_values})
end

function Matrix:dot(other)
    local new_data = {}
    local rows = self.rows
    local cols_a = self.cols
    local cols_b = other.cols
    for i = 1, rows do
        new_data[i] = {}
        for j = 1, cols_b do
            local sum = 0
            for k = 1, cols_a do
                local v1 = self.data[i][k]
                local v2 = other.data[k][j]
                sum = sum + v1 * v2
            end
            new_data[i][j] = sum
        end
    end
    return Matrix:new(new_data, rows, cols_b)
end

function Matrix.empty(rows, cols)
    return Matrix.init(rows, cols,
        function() return 0 end)
end

function Matrix:exp()
    return self:map(function(x) return exp(x) end)
end

function Matrix:flatten(axis)
    local axis, data, id = axis or 0, {{}}, 1
    local rows, cols = self.rows, self.cols
    for row, col in Iterator(rows, cols) do
        local ci = axis == 0 and 1 or id
        local cj = axis == 0 and id or 1
        data[ci], id = data[ci] or {}, id + 1
        data[ci][cj] = self.data[row][col]
    end
    return Matrix:new(data)
end

function Matrix:get(row, col)
    return self.data[row][col]
end

function Matrix.init(rows, cols, func)
    local data = {}
    for row, col in Iterator(rows, cols) do
        data[row] = data[row] or {}
        data[row][col] = func(row, col)
    end
    return Matrix:new(data, rows, cols)
end

function Matrix:log()
    return self:map(function(x) return log(x) end)
end

function Matrix:map(func)
    local rows, cols = self.rows, self.cols
    return Matrix.init(rows, cols, function(i, j)
        return func(self.data[i][j], i, j) end)
end

function Matrix:max(axis, seed)
    local seed = seed or -huge
    return self:reduce(axis or nil, seed,
        function(a, b) return max(a, b) end)
end

function Matrix:maximum(obj)
    local mat = type(obj) ~= "number"
    local rows, cols = self.rows, self.cols
    return Matrix.init(rows, cols, function(i, j)
        local val = mat and obj.data[i][j] or obj
        return max(val, self.data[i][j]) end)
end

function Matrix:mean(axis)
    return self:sum(axis) / self.size
end

function Matrix:min(axis, seed)
    local seed = seed or huge
    return self:reduce(axis or nil, seed,
        function(a, b) return min(a, b) end)
end

function Matrix:minimum(obj)
    local mat = type(obj) ~= "number"
    local rows, cols = self.rows, self.cols
    return Matrix.init(rows, cols, function(i, j)
        local val = mat and obj.data[i][j] or obj
        return min(val, self.data[i][j]) end)
end

function Matrix.ones(rows, cols)
    return Matrix.init(rows, cols,
        function() return 1 end)
end

function Matrix:ones_like()
    return self:map(function() return 1 end)
end

function Matrix:product(axis)
    return self:reduce(axis or nil, 1,
        function(a, b) return a * b end)
end

function Matrix:ptp(axis)
    return self:max(axis) - self:min(axis)
end

function Matrix.randn(rows, cols, mean, std)
    local mean, std = mean or 0, std or 1
    return Matrix.init(rows, cols, function()
        local u1, u2 = random(), random()
        local t1 = sqrt(-2 * log(1 - u1))
        local t2 = cos(2 * pi * (1 - u2))
        return mean + std * t1 * t2 end)
end

function Matrix.random(rows, cols)
    return Matrix.init(rows, cols,
        function() return random() end)
end

function Matrix.range(...)
    local args = {...}
    local values, index = {}, 1
    local step = args[3] or 1
    local start = args[1] or 0
    local stop = args[2] or args[1]
    local current = args[2] and start or 0
    local func = function(curr, stop, step)
        return step > 0 and curr < stop
        or step < 0 and curr > stop end
    while func(current, stop, step) do
        values[index] = current
        current = current + step
        index = index + 1
    end
    return Matrix:new({values})
end

function Matrix:reduce(axis, init, func)
    local data, seed = {{}}, init
    local rows, cols = self.rows, self.cols
    local si = axis == 0 and cols or rows
    local sj = axis == 0 and rows or cols
    for i, j, rst in Iterator(si, sj) do
        local ci = axis == 0 and j or i
        local cj = axis == 0 and i or j
        local value = self.data[ci][cj]
        seed = func(seed, value, ci, cj)
        if axis ~= nil and rst == true then
            local row = axis == 0 and 1 or ci
            local col = axis == 0 and cj or 1
            data[row] = data[row] or {}
            data[row][col] = seed; seed = init
        end
    end
    return axis and Matrix:new(data) or seed
end

function Matrix:reshape(new_rows, new_cols)
    local rows, cols = self.rows, self.cols
    local iter = Iterator(rows, cols)
    return Matrix.init(new_rows, new_cols,
        function() local row, col = iter()
        return self.data[row][col] end)
end

function Matrix:set(row, col, val)
    self.data[row][col] = val
end

function Matrix:sign()
    return self:map(function(x) return sign(x) end)
end

function Matrix:sinh()
    return self:map(function(x) return sinh(x) end)
end

function Matrix:sqrt()
    return self:map(function(x) return sqrt(x) end)
end

function Matrix:std(axis)
    return sqrt(self:var(axis))
end

function Matrix:sub(start_row, end_row, start_col, end_col)
    local sub_data = {}
    for i = start_row, end_row do
        local k = i - start_row + 1
        for j = start_col, end_col do
            local l = j - start_col + 1
            sub_data[k] = sub_data[k] or {}
            sub_data[k][l] = self.data[i][j]
        end
    end
    return Matrix:new(sub_data)
end

function Matrix:sum(axis)
    return self:reduce(axis or nil, 0,
        function(t, x) return t + x end)
end

function Matrix:tanh()
    return self:map(function(x) return tanh(x) end)
end

function Matrix:transpose()
    return Matrix.init(self.cols,
        self.rows, function(row, col)
        return self.data[col][row] end)
end

function Matrix:var(axis)
    local mean = self:mean(axis)
    local size, m = self.size, mean
    if axis == 0 then size = self.cols end
    if axis == 1 then size = self.rows end
    return self:reduce(axis, 0, function(t, x, i, j)
        if axis == 0 then m = mean.data[1][i] end
        if axis == 1 then m = mean.data[j][1] end
        return t + (x - m) ^ 2 end) / size
end

function Matrix.zeros(rows, cols)
    return Matrix.empty(rows, cols)
end

function Matrix:zeros_like()
    return self:map(function() return 0 end)
end

---------------------
---- Activations ----

local Activation = Class()

function Activation:__init() end

function Activation:Calculate(input)
    return input
end

function Activation:Derivative(input)
    return 1
end

-- Binary step activation

local BinaryStep = Class(Activation)

function BinaryStep:__init()
    Activation.__init(self)
end

function BinaryStep:Calculate(input)
    return input:map(function(x)
        return x < 0 and 0 or 1 end)
end

function BinaryStep:Derivative(input)
    return 0
end

-- ELU activation

local ELU = Class(Activation)

function ELU:__init(alpha)
    Activation.__init(self)
    self.alpha = alpha or 1.0
end

function ELU:Calculate(input)
    return input:map(function(x) return x < 0
        and self.alpha * (exp(x) - 1) or x end)
end

function ELU:Derivative(input)
    return input:map(function(x) return x < 0
        and self.alpha * exp(x) or 1 end)
end

-- GELU activation

local GELU = Class(Activation)

function GELU:__init(alpha)
    Activation.__init(self)
    self.c1 = sqrt(2) * 0.5
    self.c2 = sqrt(2 / pi)
end

function GELU:Calculate(input)
    return input:map(function(x) return x *
        0.5 * (1 + erf(self.c1 * x)) end)
end

function GELU:Derivative(input)
    return input:map(function(x)
        local ef = erf(self.c1 * x)
        local ex = exp(-0.5 * x * x)
        local term = self.c2 * ex * x
        return 0.5 * (ef + term + 1)
    end)
end

-- Leaky ReLU activation

local LeakyReLU = Class(Activation)

function LeakyReLU:__init(alpha)
    Activation.__init(self)
    self.alpha = alpha or 0.01
end

function LeakyReLU:Calculate(input)
    return input:map(function(x) return
        x > 0 and x or self.alpha * x end)
end

function LeakyReLU:Derivative(input)
    return input:map(function(x) return
        x > 0 and 1 or self.alpha end)
end

-- Mish activation

local Mish = Class(Activation)

function Mish:__init(alpha)
    Activation.__init(self)
end

function Mish:Calculate(input)
    return input:map(function(x) local ex = exp(x)
        return x - 2 * x / (ex * (ex + 2) + 2) end)
end

function Mish:Derivative(input)
    return input:map(function(x)
        local ex, x4 = exp(x), (4 * x)
        local den = 2 * ex + exp(2 * x) + 2
        local na, nb = x4 - 2, x4 * (ex + 2)
        return na / den - nb / (den ^ 2) + 1
    end)
end

-- ReLU activation

local ReLU = Class(Activation)

function ReLU:__init()
    Activation.__init(self)
end

function ReLU:Calculate(input)
    return input:maximum(0)
end

function ReLU:Derivative(input)
    return input:map(function(x)
        return x > 0 and 1 or 0 end)
end

-- SELU activation

local SELU = Class(Activation)

function SELU:__init(alpha, scale)
    Activation.__init(self)
    self.alpha = alpha or 1.67326324
    self.scale = scale or 1.05070098
end

function SELU:Calculate(input)
    return input:map(function(x) return self.scale *
        (x > 0 and x or self.alpha * (exp(x) - 1)) end)
end

function SELU:Derivative(input)
    return input:map(function(x) return self.scale *
        (x > 0 and 1 or self.alpha * exp(x)) end)
end

-- Sigmoid activation

local Sigmoid = Class(Activation)

function Sigmoid:__init()
    Activation.__init(self)
end

function Sigmoid:Calculate(input)
    return 1 / (1 + (-input):exp())
end

function Sigmoid:Derivative(input)
    local fx = self:Calculate(input)
    return fx * (1 - fx)
end

-- SiLU activation

local SiLU = Class(Activation)

function SiLU:__init()
    Activation.__init(self)
end

function SiLU:Calculate(input)
    return input / (1 + (-input):exp())
end

function SiLU:Derivative(input)
    local fx = 1 / (1 + (-input):exp())
    return input * fx * (1 - fx) + fx
end

-- Softmax activation

local Softmax = Class(Activation)

function Softmax:__init()
    Activation.__init(self)
end

function Softmax:Calculate(input)
    local max = nums:max(1)
    local exps = (input - max):exp()
    return exps / exps:sum(1)
end

function Softmax:Derivative(input)
    local fx = self:Calculate(input)
    return fx * (1 - fx)
end

-- Softplus activation

local Softplus = Class(Activation)

function Softplus:__init()
    Activation.__init(self)
end

function Softplus:Calculate(input)
    return (1 + input:exp()):log()
end

function Softplus:Derivative(input)
    return 1 / (1 + (-input):exp())
end

-- Softsign activation

local Softsign = Class(Activation)

function Softsign:__init()
    Activation.__init(self)
end

function Softsign:Calculate(input)
    return input / (input:abs() + 1)
end

function Softsign:Derivative(input)
    return 1 / (input:abs() + 1) ^ 2
end

-- Tanh activation

local Tanh = Class(Activation)

function Tanh:__init()
    Activation.__init(self)
end

function Tanh:Calculate(input)
    return input:tanh()
end

function Tanh:Derivative(input)
    local fx = self:Calculate(input)
    return 1 - fx * fx
end

--------------------
------ Losses ------

local Loss = Class()

function Loss:__init()
    self.epsilon = 1e-7
end

function Loss:Calculate(target, pred)
    return pred - target
end

function Loss:Derivative(target, pred)
    return 0
end

-- Binary cross entropy

local CrossEntropy = Class(Loss)

function CrossEntropy:__init()
    Loss.__init(self)
end

function CrossEntropy:Calculate(target, pred)
    local pred = pred + self.epsilon
    return -(target * pred:log() + (1 -
        target) * (1 - pred):log()):mean()
end

function CrossEntropy:Derivative(target, pred)
    local factor = -1 / target.size
    local pred = pred + self.epsilon
    return factor * (target / pred -
        (1 - target) / (1 - pred))
end

-- Hinge loss

local Hinge = Class(Loss)

function Hinge:__init()
    Loss.__init(self)
    self.convert = function(x)
        return x == 0 and -1 or x end
end

function Hinge:Calculate(target, pred)
    local target = target:map(self.convert)
    return (1 - pred * target):maximum(0):mean()
end

function Hinge:Derivative(target, pred)
    local target = target:map(self.convert)
    return (pred * target):map(function(
        val, row, col) return val >= 1 and 0 or
        -target:get(row, col) end) / target.size
end

-- Huber loss

local Huber = Class(Loss)

function Huber:__init(delta)
    Loss.__init(self)
    self.delta = delta or 1.0
end

function Huber:Calculate(target, pred)
    return (pred - target):map(function(x) return
        abs(x) <= self.delta and 0.5 * x * x or (abs(x)
        - 0.5 * self.delta) * self.delta end):mean()
end

function Huber:Derivative(target, pred)
    return (pred - target):map(function(x)
        return abs(x) <= self.delta and x or
        self.delta * sign(x) end) / target.size
end

-- Log Cosh loss

local LogCosh = Class(Loss)

function LogCosh:__init()
    Loss.__init(self)
end

function LogCosh:Calculate(target, pred)
    return (pred - target):cosh():log():mean()
end

function LogCosh:Derivative(target, pred)
    return (pred - target):tanh() / target.size
end

-- Mean absolute error (MAE)

local MeanAbsoluteError = Class(Loss)

function MeanAbsoluteError:__init()
    Loss.__init(self)
end

function MeanAbsoluteError:Calculate(target, pred)
    return (pred - target):abs():mean()
end

function MeanAbsoluteError:Derivative(target, pred)
    return (pred - target):sign() / target.size
end

-- Mean squared error (MSE)

local MeanSquaredError = Class(Loss)

function MeanSquaredError:__init()
    Loss.__init(self)
end

function MeanSquaredError:Calculate(target, pred)
    return ((pred - target) ^ 2):mean()
end

function MeanSquaredError:Derivative(target, pred)
    return 2 * (pred - target) / target.size
end

-- Mean squared log error (MSLE)

local MeanSquaredLogError = Class(Loss)

function MeanSquaredLogError:__init()
    Loss.__init(self)
end

function MeanSquaredLogError:Calculate(target, pred)
    local pred = pred + (1 + self.epsilon)
    local target = target + (1 + self.epsilon)
    return ((pred:log() - target:log()) ^ 2):mean()
end

function MeanSquaredLogError:Derivative(target, pred)
    local factor = 2 / target.size
    local target = target + self.epsilon
    return (pred / target - 1) * factor
end

--------------------
----- Accuracy -----

local Accuracy = Class()

function Accuracy:__init() end

function Accuracy:Calculate(target, pred)
    return (pred - target):map(function(x)
        return x == 0 and 1 or 0 end):mean()
end

-- Binary model

local Binary = Class(Accuracy)

function Binary:__init()
    Accuracy.__init(self)
end

function Binary:Calculate(target, pred)
    local pred = pred:map(function(x)
        return x >= 0.5 and 1 or 0 end)
    return (pred - target):map(function(x)
        return x == 0 and 1 or 0 end):mean()
end

-- Classification model

local Classification = Class(Accuracy)

function Classification:__init()
    Accuracy.__init(self)
end

function Classification:Calculate(target, pred)
    local pred = pred:argmax(1)
    local target = target:argmax(1)
    return (pred - target):map(function(x)
        return x == 0 and 1 or 0 end):mean()
end

-- Regression model

local Regression = Class(Accuracy)

function Regression:__init(precision)
    Accuracy.__init(self)
    self.precision = precision or 0.01
end

function Regression:Calculate(target, pred)
    local func = function(x) return abs(x)
        <= self.precision and 1 or 0 end
    return (pred - target):map(func):mean()
end

---------------------
----- Optimizer -----

local Optimizer = Class()

function Optimizer:__init(args)
    local args = args or {}
    self.decay = args.decay or 0
    self.layer_index = 0
    self.iteration = 0
end

function Optimizer:OnPreUpdate()
    if self.decay == 0 then return end
    -- Update the learning rate with weight decay
    -- A rate is reduced as training progresses
    self.curr_learn_rate = self.learn_rate *
        (1 / (1 + self.iteration * self.decay))
end

function Optimizer:OnPostUpdate()
    -- Increment iteration after updates
    self.iteration = self.iteration + 1
end

function Optimizer:SetLayerIndex(index)
    self.layer_index = index
end

function Optimizer:UpdateWeights(layer)
    local dw = -self.curr_learn_rate * layer.dw
    layer.weights = layer.weights + dw
end

function Optimizer:UpdateBias(layer)
    local db = -self.curr_learn_rate * layer.db
    layer.bias = layer.bias + db
end

-- Stochastic gradient descent

local SGD = Class(Optimizer)

function SGD:__init(args)
    local args = args or {}
    Optimizer.__init(self, args)
    self.learn_rate = args.learn_rate or 0.01
    self.curr_learn_rate = self.learn_rate
    self.momentum = args.momentum or 0
end

function SGD:UpdateWeights(layer)
    local momentum = self.momentum
    local boost = momentum ~= 0
    -- Calculate the weight update
    local dw = -self.curr_learn_rate * layer.dw
    layer.mw = layer.mw or layer.dw:zeros_like()
    -- Apply momentum term to the update if enabled
    if boost then dw = dw + momentum * layer.mw end
    layer.weights, layer.mw = layer.weights + dw, dw
end

function SGD:UpdateBias(layer)
    local momentum = self.momentum
    local boost = momentum ~= 0
    -- Calculate the weight update
    local db = -self.curr_learn_rate * layer.db
    layer.mb = layer.mb or layer.db:zeros_like()
    -- Apply momentum term to the update if enabled
    if boost then db = db + momentum * layer.mb end
    layer.bias, layer.mb = layer.bias + db, db
end

-- Adagrad optimizer

local Adagrad = Class(Optimizer)

function Adagrad:__init(args)
    local args = args or {}
    Optimizer.__init(self, args)
    self.learn_rate = args.learn_rate or 0.001
    self.curr_learn_rate = self.learn_rate
    self.epsilon = args.epsilon or 1e-7
end

function Adagrad:UpdateWeights(layer)
    -- Update cache of squared weight gradients
    layer.cw = layer.cw or layer.dw:zeros_like()
    layer.cw = layer.cw + layer.dw * layer.dw
    -- Calculate the update using Adagrad theory
    local dw = -self.curr_learn_rate * layer.dw
    dw = dw / (layer.cw:sqrt() + self.epsilon)
    layer.weights = layer.weights + dw
end

function Adagrad:UpdateBias(layer)
    -- Update cache of squared bias gradients
    layer.cb = layer.cb or layer.db:zeros_like()
    layer.cb = layer.cb + layer.db * layer.db
    -- Calculate the update using Adagrad theory
    local db = -self.curr_learn_rate * layer.db
    db = db / (layer.cb:sqrt() + self.epsilon)
    layer.bias = layer.bias + db
end

-- Adam optimizer

local Adam = Class(Optimizer)

function Adam:__init(args)
    local args = args or {}
    Optimizer.__init(self, args)
    self.learn_rate = args.learn_rate or 0.001
    self.curr_learn_rate = self.learn_rate
    self.epsilon = args.epsilon or 1e-7
    self.beta_1 = args.beta_1 or 0.9
    self.beta_2 = args.beta_2 or 0.999
end

function Adam:UpdateWeights(layer)
    local iter = self.iteration + 1
    local b1, b2 = self.beta_1, self.beta_2
    -- Update momentum and cache with gradients
    layer.mw = layer.mw or layer.dw:zeros_like()
    layer.cw = layer.cw or layer.dw:zeros_like()
    layer.mw = b1 * layer.mw + (1 - b1) * layer.dw
    layer.cw = b2 * layer.cw + (1 - b2) * layer.dw ^ 2
    -- Calculate the corrected momentum and cache
    local corr_mw = layer.mw / (1 - b1 ^ iter)
    local corr_cw = layer.cw / (1 - b2 ^ iter)
    -- Calculate the update using Adam theory
    local dw = -self.curr_learn_rate * corr_mw
    dw = dw / (corr_cw:sqrt() + self.epsilon)
    layer.weights = layer.weights + dw
end

function Adam:UpdateBias(layer)
    local iter = self.iteration + 1
    local b1, b2 = self.beta_1, self.beta_2
    -- Update momentum and cache with gradients
    layer.mb = layer.mb or layer.db:zeros_like()
    layer.cb = layer.cb or layer.db:zeros_like()
    layer.mb = b1 * layer.mb + (1 - b1) * layer.db
    layer.cb = b2 * layer.cb + (1 - b2) * layer.db ^ 2
    -- Calculate the corrected momentum and cache
    local corr_mb = layer.mb / (1 - b1 ^ iter)
    local corr_cb = layer.cb / (1 - b2 ^ iter)
    -- Calculate the update using Adam theory
    local db = -self.curr_learn_rate * corr_mb
    db = db / (corr_cb:sqrt() + self.epsilon)
    layer.bias = layer.bias + db
end

-- RMSprop optimizer

local RMSprop = Class(Optimizer)

function RMSprop:__init(args)
    local args = args or {}
    Optimizer.__init(self, args)
    self.learn_rate = args.learn_rate or 0.001
    self.curr_learn_rate = self.learn_rate
    self.epsilon = args.epsilon or 1e-7
    self.rho = args.rho or 0.9
end

function RMSprop:UpdateWeights(layer)
    local rho, sqr = self.rho, layer.dw ^ 2
    -- Update cache of squared weight gradients
    layer.cw = layer.cw or layer.dw:zeros_like()
    layer.cw = rho * layer.cw + (1 - rho) * sqr
    -- Calculate the update using RMSprop theory
    local dw = -self.curr_learn_rate * layer.dw
    dw = dw / (layer.cw:sqrt() + self.epsilon)
    layer.weights = layer.weights + dw
end

function RMSprop:UpdateBias(layer)
    local rho, sqr = self.rho, layer.db ^ 2
    -- Update cache of squared bias gradients
    layer.cb = layer.cb or layer.db:zeros_like()
    layer.cb = rho * layer.cb + (1 - rho) * sqr
    -- Calculate the update using RMSprop theory
    local db = -self.curr_learn_rate * layer.db
    db = db / (layer.cb:sqrt() + self.epsilon)
    layer.bias = layer.bias + db
end

---------------------
------- Layer -------

local Layer = Class()

function Layer:__init(nodes_in, nodes_out, activation)
    -- Set number of input and output nodes for the layer
    self.nodes_in, self.nodes_out = nodes_in, nodes_out
    -- Set the activation function for the layer
    self.activation = activation or ReLU:new()
    -- Init random weights and zero-value bias
    self.weights = Matrix.randn(nodes_in,
        nodes_out) * sqrt(2.0 / nodes_in)
    self.bias = Matrix.zeros(1, nodes_out)
    -- Store the weight and bias updates
    self.dw = Matrix.zeros(nodes_in, nodes_out)
    self.db = Matrix.zeros(1, nodes_out)
    -- Store the input and forward output
    self.input, self.output = nil, nil
end

function Layer:Backward(dx, index)
    -- Apply derivative of the activation function
    local input, output = self.input, self.output
    dx = self.activation:Derivative(output) * dx
    -- Compute gradients for weights and bias
    local dw = input:transpose():dot(dx)
    self.dw, self.db = dw, dx:sum(0)
    -- Return if reached the first layer
    if index <= 1 then return nil end
    -- Recalculate delta for next iteration
    return dx:dot(self.weights:transpose())
end

function Layer:Forward(input)
    -- Perform a feed-forward operation on input
    local weights_sum = input:dot(self.weights)
    local output = weights_sum + self.bias
    self.input, self.output = input, output
    return self.activation:Calculate(output)
end

----------------------
--- Neural Network ---

local NeuralNetwork = Class()

function NeuralNetwork:__init(args)
    self.layers = args.layers
    self.num_layers = #self.layers
    self.epochs = max(1, args.epochs or 100)
    self.batch_size = max(1, args.batch_size or 32)
    self.loss = args.loss or MeanSquaredError:new()
    self.accuracy = self.accuracy or Binary:new()
    self.optimizer = args.optimizer or SGD:new()
    self.epochcb, self.stop = nil, false
end

function NeuralNetwork:Accuracy(target, pred)
    return self.accuracy:Calculate(target, pred)
end

function NeuralNetwork:Backpropagate(input, target, pred)
    -- Calculate loss gradient with respect to outputs
    local grad = self.loss:Derivative(target, pred)
    -- Backpropagate the gradients through layers
    for index = self.num_layers, 1, -1 do
        local layer = self.layers[index]
        grad = layer:Backward(grad, index)
    end
    -- Pre-processing before updates
    self.optimizer:OnPreUpdate()
    -- Update the weights and biases
    for index = 1, self.num_layers do
        local layer = self.layers[index]
        self.optimizer:SetLayerIndex(index)
        self.optimizer:UpdateWeights(layer)
        self.optimizer:UpdateBias(layer)
    end
    -- Post-processing after updates
    self.optimizer:OnPostUpdate()
end

function NeuralNetwork:BuildBatches(input, target)
    local batches = {}
    for batch = 1, ceil(input.rows / self.batch_size) do
        -- Calculate the start and end indices for the batch
        local start_idx = (batch - 1) * self.batch_size + 1
        local end_idx = min(start_idx + self.batch_size - 1, input.rows)
        -- Extract the input and expected target for the current batch
        local batch_input = input:sub(start_idx, end_idx, 1, input.cols)
        local batch_target = target:sub(start_idx, end_idx, 1, target.cols)
        batches[batch] = {input = batch_input, target = batch_target}
    end
    return batches
end

function NeuralNetwork:LoadWeights(string)
    local data = load(string)()
    for index = 1, self.num_layers do
        local layer = self.layers[index]
        local weights = data[index]["weights"]
        local bias = data[index]["bias"]
        layer.weights.data = weights
        layer.bias.data = bias
    end
end

function NeuralNetwork:Loss(target, pred)
    return self.loss:Calculate(target, pred)
end

function NeuralNetwork:OnEpochEnd(callback)
    self.epochcb = callback
end

function NeuralNetwork:Predict(input)
    local output = input
    for index = 1, self.num_layers do
        local layer = self.layers[index]
        output = layer:Forward(output)
    end
    return output
end

function NeuralNetwork:SaveWeights()
    local data = {}
    for index = 1, self.num_layers do
        local layer = self.layers[index]
        local weights = layer.weights.data
        local bias = layer.bias.data
        data[index] = data[index] or {}
        data[index]["weights"] = weights
        data[index]["bias"] = bias
    end
    return Serialize(data)
end

function NeuralNetwork:Train(input, target)
    -- Divide dataset into individual mini-batches
    local batches = self:BuildBatches(input, target)
    local num_batches = #batches; self.stop = false
    for epoch = 1, self.epochs do
        for batch = 1, num_batches do
            local batch_input = batches[batch].input
            local batch_target = batches[batch].target
            -- Feed data through network to return outputs
            local prediction = self:Predict(batch_input)
            -- Perform backpropagation to update weights and bias
            self:Backpropagate(batch_input, batch_target, prediction)
        end
        -- Predict using the entire input dataset
        local prediction = self:Predict(input)
        -- Calculate the loss over entire dataset
        local loss = self:Loss(target, prediction)
        -- Calculate the accuracy using comparison function
        local accuracy = self:Accuracy(target, prediction)
        -- Generate and print statistics for the current epoch
        local stats = format("Epoch: %d Loss: %f", epoch, loss)
        prnt("\r" .. stats .. format(" Accuracy %.4f", accuracy))
        -- Fire the epoch end callback if it's set on user's end
        if self.epochcb then self.epochcb(epoch, accuracy) end
        -- Break the training if a stop flag is set to true
        if self.stop == true then return end
    end
end

return {
    Accuracy = Accuracy,
    Activation = Activation,
    Adagrad = Adagrad,
    Adam = Adam,
    Binary = Binary,
    BinaryStep = BinaryStep,
    Classification = Classification,
    CrossEntropy = CrossEntropy,
    ELU = ELU,
    GELU = GELU,
    Hinge = Hinge,
    Huber = Huber,
    Layer = Layer,
    LeakyReLU = LeakyReLU,
    LogCosh = LogCosh,
    Loss = Loss,
    Matrix = Matrix,
    MeanAbsoluteError = MeanAbsoluteError,
    MeanSquaredError = MeanSquaredError,
    MeanSquaredLogError = MeanSquaredLogError,
    Mish = Mish,
    NeuralNetwork = NeuralNetwork,
    Optimizer = Optimizer,
    ReLU = ReLU,
    Regression = Regression,
    RMSprop = RMSprop,
    SELU = SELU,
    Sigmoid = Sigmoid,
    SiLU = SiLU,
    SGD = SGD,
    Softmax = Softmax,
    Softplus = Softplus,
    Softsign = Softsign,
    Tanh = Tanh,
}

