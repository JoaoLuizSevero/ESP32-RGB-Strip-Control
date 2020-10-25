const { series } = require('gulp');
var gulp = require('gulp')
var plumber = require('gulp-plumber')
var htmlmin = require('gulp-htmlmin')
var cleancss = require('gulp-clean-css')
var uglify = require('gulp-uglify')
var gzip = require('gulp-gzip')
var del = require('del')
var useref = require('gulp-useref')
var gulpif = require('gulp-if')
var inline = require('gulp-inline')
 
var srcPath = 'html/*.html'
var destPath = 'ffs/data'

function processPage() {
    del([destPath+'/index.html.gz'])
    return gulp.src(srcPath)
        .pipe(useref())
        .pipe(plumber())
        .pipe(gulpif('*.css', cleancss()))
        .pipe(gulpif('*.js', uglify()))
        .pipe(gulpif('*.html', htmlmin({
            collapseWhitespace: true,
            removeComments: true,
            minifyCSS: true,
            minifyJS: true
        })))
        .pipe(gzip())
        .pipe(gulp.dest(destPath));
}

exports.default = series(processPage);