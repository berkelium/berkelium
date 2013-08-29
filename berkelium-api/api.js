var langs = {
	'cpp': 'C++',
	'java': 'Java',
	'c': 'C',
};

var hint = {};

$('hint').each(function(key, value) {
	var q = $(value);
	var m = q.attr('match');
	hint[m] = {
		title:$.trim($('title', q).html()),
		content:$.trim($('content', q).html()),
		type:q.attr('type') || 'info',
		match:m,
		re:new RegExp(m, 'g'),
	};
});

function color(color) {
	var re = new RegExp('/\\*' + color + '\\*/(.*)/\\*' + color + '\\*/', 'g');
	color = color.toLowerCase();

	return function(txt) {
		return txt.replace(re, '<span class="' + color + '">$1</span>');
	}
}

var colors = [
	color('GREEN'),
	color('YELLOW'),
	color('RED'),
	color('BLUE'),
];

for(var file in source) {
	var txt = source[file];
	txt = txt.replace(/&/g, "&amp;");
	txt = txt.replace(/</g, "&lt;");
	txt = txt.replace(/>/g, "&gt;");
	for(var c in colors) {
		txt = colors[c](txt);
	}
	for(var h in hint) {
		txt = txt.replace(hint[h].re, '<tooltip class="' + hint[h].type + '" match="' + hint[h].match + '">$1</tooltip>');
	}
	source[file] = txt;
}

function fixPosition(id, top1, win1)
{
	var top2 = id.position().top;
	var win2 = $(window).scrollTop();

	var w = $(window);
	w.scrollTop(w.scrollTop() + top2 - top1 - win2 + win1);
}

function selectSource(type, hash) {
	var later = undefined;
	if(hash) {
		var id = $('#' + hash);
		id.removeAttr('id');
		location.hash = hash;
		id.attr('id', hash);
		var top1 = id.position().top;
		var win1 = $(window).scrollTop();
		later = function() {
			fixPosition(id, top1, win1);
		};
	}
	$('.tab-' + type + ' a').tab('show');
	$('.source').addClass('hidden');
	$('.source-' + type).removeClass('hidden');
	$('#sidebar a').each(function(key, value) {
		var v = $(value);
		var t = v.attr("href").split("-");
		if(t[1]) {
			v.attr("href", "#" + type + '-' + t[1] + (t[2] ? '-' + t[2] : ''));
		}
	});
	$('body').scrollspy({target:'#sidebar'});
	$('body').scrollspy('refresh');
	if(later) {
		$(later);
	}
}

function panel(div)
{
	var parent = div.parent();
	var type = '';
	var text = '';
	var lang = '';
	if(parent.is('file')) {
		lang = langs[parent.attr('type')] + ' ';
	}
	if(div.is('.info')) {
		type = 'primary';
		text = 'Info';
	} else if(div.is('.warn')) {
		type = 'danger';
		text = 'Warning';
	}
	if(type != '') {
		var ret = '<div class="panel panel-' + type + '">';
		ret += '<div class="panel-heading">';
		ret += '<h3 class="panel-title">' + lang + text + '</h3>';
		ret += '</div>';
		ret += '<div class="panel-body">';
		ret += div.html();
		ret += '</div>';
		ret += '</div>';
		return $(ret);
	}
	return div;
}

function buildContent()
{
	var sidebar = '';
	var section = '';
	$('h1,entry').each(function(key, value) {
		var q = $(value);
		var id = q.attr('id');
		for(l in langs) {
			q.before('<div id="' + l + '-' + id + '"></div>');
		}
		if(q.is('h1')) {
			if(sidebar != "") {
				sidebar += '</ul></li>';
			}
			section = q.text();
			sidebar += '<li><a href="#cpp-' + id + '">' + section + '</a><ul class="nav">';
		} else {
			var t = $('h2', q);
			sidebar += '<li><a href="#cpp-' + id + '">' + $.trim(t.text()) + '</a></li>';
			t.text(section + ' - ' + t.text());
			q.before(t);
			$('> div, file > div', q).each(function(key, value) {
				q.before(panel($(value)));
			});

			// tabs
			var tabs = '<ul class="nav nav-tabs">';
			var first = true;
			for(l in langs) {
				tabs += '<li class="';
				if(first) {
					tabs += "active ";
					first = false;
				}
				tabs += 'tab-' + l + '">';
				tabs += '<a onclick="selectSource(' + "'" + l + "', '" + l + '-' + id + "'" + ')">';
				tabs += langs[l];
				tabs += '</a></li>';
			}
			tabs += '</ul>';

			// tab content
			tabs += '<div class="tab-content">';
			first = true;
			for(l in langs) {
				tabs += '<div class="source source-' + l;
				if(first) {
					first = false;
				} else {
					tabs += ' hidden';
				}
				tabs += '">';
				var name = $('file[type=' + l + ']', q).attr('name');
				var file = source[name];
				tabs += '<pre class="prettyprint linenums">';
				if(file) {
					tabs += file;
				} else {
					tabs += "// source not found...";
				}
				tabs += '</pre>';
				if(name) {
					tabs += '<div class="alert alert-info"> Source: <a href="../' + name + '">' + name + '</a></div>';
				}
				tabs += '</div>';
			}
			tabs += '</div><br/>';

			q.before(tabs);
			q.remove();
		}
	});
	sidebar += '</ul></li>';
	$('#sidebar ul').append(sidebar);

	for(var l in langs) {
		if(location.hash.substr(1, l.length) == l) {
			$(function() {
				selectSource(l);
			});
			break;
		}
	}
	$('tooltip').each(function(key, value) {
		var q = $(value);
		var text = q.text();
		var h = hint[q.attr('match')];

		var opts = {
			placement:'top',
			html:true,
			container:'#popover-' + h.type,
			trigger:'hover',
			title:h.title,
			content:h.content
		};
		q.popover(opts);
		var clicked = true;
		q.click(function() {
			q.popover('hide');
			if(clicked) {
				opts.trigger = 'manual';
			} else {
				opts.trigger = 'hover';
			}
			q.popover('destroy');
			q.popover(opts);
			q.popover('show');
			clicked = !clicked;
		});
	});
	prettyPrint();
}

buildContent();
