local intl = require 'intl'

print(intl.bindtextdomain('test'))

print(intl.textdomain())
print(intl.textdomain('testintl'))
print(intl.textdomain())

print(intl.dgettext(nil, 'hello'))
