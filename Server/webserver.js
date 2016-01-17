/**
 * 
 */

function WebServer()
{
	var express 	= require("express"),
		swig		= require("swig"),
		http		= require("http"),
		app			= express(),
		instance 	= null;

	var defaults = {
		port: 8080,
		host: '0.0.0.0',
		expressCache: false,
		swigCache: false
	};
			
	WebServer.prototype.startServer = function(swigViewsPath, staticPath)
	{
		// define the swig engine
		app.engine('html', swig.renderFile);
		app.set('view engine', 'html');
		app.set('views', swigViewsPath);

		// set cache
		app.set('view cache', defaults.expressCache);
		swig.setDefaults({ cache: defaults.swigCache });

		// listen on the root /
		app.get('/', function(req, res){
			res.render('index');
		});

		// bind static resources
		app.use(express.static(staticPath));

		// define the instance
		instance = app.listen(defaults.port, defaults.host);
		
		console.log("Server started");
	};
	
	WebServer.prototype.getInstance = function()
	{
		return instance;
	};
}

module.exports = new WebServer();